#include <wheels/test/test_framework.hpp>
#include <tinyfibers/test/test.hpp>

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/sync/wait_group.hpp>
#include <tinyfibers/sync/mutex.hpp>
#include <tinyfibers/sync/condvar.hpp>

#include <wheels/support/time.hpp>

#include <memory>
#include <chrono>

using namespace tinyfibers;

using namespace std::chrono_literals;

TEST_SUITE(Fibers) {

SIMPLE_TEST(JustWorks) {
  RunScheduler([]() {
    self::Yield();
  });
}

SIMPLE_TEST(Join) {
  RunScheduler([]() {
    bool done = false;
    JoinHandle h = Spawn([&]() {
      done = true;
    });
    ASSERT_FALSE(done);
    h.Join();
    ASSERT_TRUE(done);
  });
}

SIMPLE_TEST(JoinCompleted) {
  RunScheduler([]() {
    bool done = false;
    JoinHandle h = Spawn([&]() {
      done = true;
    });
    self::Yield();
    ASSERT_TRUE(done);
    h.Join();
  });
}

// At least does not panic
SIMPLE_TEST(Detach) {
  RunScheduler([]() {
    Spawn([&]() {
      self::Yield();
    }).Detach();
  });
}

SIMPLE_TEST(Ids) {
  RunScheduler([]() {
    FiberId main_id = self::GetId();

    auto finn = [&]() {
      ASSERT_EQ(self::GetId(), main_id + 1);
    };

    auto jake = [&]() {
      ASSERT_EQ(self::GetId(), main_id + 2);
    };

    JoinHandle f1 = Spawn(finn);
    JoinHandle f2 = Spawn(jake);

    self::Yield();

    ASSERT_EQ(main_id, self::GetId());

    f1.Join();
    f2.Join();
  });
}

SIMPLE_TEST(PingPong) {
  int count = 0;

  auto finn = [&count]() {
    for (size_t i = 0; i < 10; ++i) {
      ++count;
      self::Yield();
      ASSERT_EQ(count, 0);
    }
    ++count;
  };

  auto jake = [&count]() {
    for (size_t i = 0; i < 10; ++i) {
      --count;
      self::Yield();
      ASSERT_EQ(count, 1);
    }
  };

  RunScheduler([&]() {
    WaitGroup wg;
    wg.Spawn(finn);
    wg.Spawn(jake);
    wg.Wait();
  });
}

SIMPLE_TEST(SleepFor) {
  RunScheduler([]() {
    wheels::StopWatch stop_watch;
    self::SleepFor(1s);
    ASSERT_GE(stop_watch.Elapsed(), 1s);
  });
}

SIMPLE_TEST(FifoScheduling) {
  static const size_t kFibers = 5;
  static const size_t kRounds = 5;

  size_t next = 0;

  auto routine = [&](size_t k) {
    for (size_t i = 0; i < kRounds; ++i) {
      ASSERT_EQ(next, k);
      next = (next + 1) % kFibers;
      self::Yield();
    }
  };

  RunScheduler([&]() {
    WaitGroup wg;
    for (size_t k = 0; k < kFibers; ++k) {
      wg.Spawn([&, k]() { routine(k); });
    }
    wg.Wait();
  });
}

SIMPLE_TEST(WaitQueue) {
  WaitQueue wait_queue;
  int step = 0;

  auto foo = [&]() {
    ASSERT_EQ(step, 1);
    wait_queue.WakeOne();
    ASSERT_EQ(step, 1);
    ++step;
    self::Yield();
    ASSERT_EQ(step, 3);
  };

  auto main = [&]() {
    auto f = Spawn(foo);
    ++step;
    wait_queue.Park();
    ASSERT_EQ(step, 2);
    ++step;
    self::Yield();
    f.Join();
  };

  RunScheduler(main);
}

SIMPLE_TEST(Mutex) {
  Mutex mutex;
  bool critical = false;

  auto routine = [&]() {
    for (size_t i = 0; i < 10; ++i) {
      mutex.Lock();
      ASSERT_FALSE(critical);
      critical = true;
      for (size_t j = 0; j < 3; ++j) {
        self::Yield();
      }
      ASSERT_TRUE(critical);
      critical = false;
      mutex.Unlock();
      self::Yield();
    }
  };

  RunScheduler([&]() {
    WaitGroup wg;
    wg.Spawn(routine);
    wg.Spawn(routine);
    wg.Wait();
  });
}

SIMPLE_TEST(MutexTryLock) {
  Mutex mutex;

  auto locker = [&mutex]() {
    mutex.Lock();
    self::Yield();
    mutex.Unlock();
  };

  auto try_locker = [&mutex]() {
    ASSERT_FALSE(mutex.TryLock());
    self::Yield();
    ASSERT_TRUE(mutex.TryLock());
    mutex.Unlock();
  };

  RunScheduler([&]() {
    WaitGroup wg;
    wg.Spawn(locker);
    wg.Spawn(try_locker);
    wg.Wait();
  });
}

SIMPLE_TEST(ConditionVariable) {
  Mutex mutex;
  ConditionVariable ready;
  std::string message;

  auto receive = [&]() {
    std::unique_lock lock(mutex);

    ready.Wait(mutex);
    ASSERT_EQ(message, "Hello");
  };

  auto send = [&]() {
    std::lock_guard guard(mutex);
    for (size_t i = 0; i < 100; ++i) {
      self::Yield();
    }
    message = "Hello";
    ready.NotifyOne();
  };

  auto init = [&]() {
    WaitGroup wg;
    wg.Spawn(receive);
    wg.Spawn(send);
    wg.Wait();
  };

  RunScheduler(init);
}

class OnePassBarrier {
 public:
  OnePassBarrier(size_t threads)
      : threads_(threads) {
  }

  void Arrive() {
    std::unique_lock lock(mutex_);
    --threads_;
    if (threads_ == 0) {
      all_arrived_.NotifyAll();
    } else {
      all_arrived_.Wait(lock, [this]() { return threads_ == 0; });
    }
  }

 private:
  size_t threads_;
  Mutex mutex_;
  ConditionVariable all_arrived_;
};

SIMPLE_TEST(Barrier) {
  static const size_t kFibers = 100;

  OnePassBarrier barrier{kFibers};
  size_t arrived = 0;

  auto participant = [&]() {
    ++arrived;
    barrier.Arrive();
    ASSERT_EQ(arrived, kFibers);
  };

  RunScheduler([&]() {
    WaitGroup wg;
    for (size_t i = 0; i < kFibers; ++i) {
      wg.Spawn(participant);
    }
    wg.Wait();
  });
}

SIMPLE_TEST(NoLeaks) {
  auto strong_ref = std::make_shared<int>(42);
  std::weak_ptr<int> weak_ref = strong_ref;

  RunScheduler([ref = std::move(strong_ref)]() {
    std::cout << "Answer to the Ultimate Question of Life, the Universe, and Everything: "
              << *ref << std::endl;
  });

  ASSERT_FALSE(weak_ref.lock());
}

}
