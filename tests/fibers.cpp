#include "gtest/gtest.h"

#include <tinyfibers/runtime/api.hpp>
#include <tinyfibers/sync/mutex.hpp>
#include <tinyfibers/sync/condvar.hpp>

#include <tinysupport/time.hpp>

#include <memory>
#include <chrono>

using namespace tiny::fibers;

using namespace std::chrono_literals;

TEST(Fibers, YieldOnce) {
  RunScheduler([]() {
    Yield();
  });
}

TEST(Fibers, Ids) {
  RunScheduler([]() {
    FiberId main_id = GetFiberId();

    auto finn = [&]() {
      ASSERT_EQ(GetFiberId(), main_id + 1);
    };

    auto jake = [&]() {
      ASSERT_EQ(GetFiberId(), main_id + 2);
    };

    Spawn(finn);
    Spawn(jake);

    Yield();

    ASSERT_EQ(main_id, GetFiberId());
  });
}

TEST(Fibers, PingPong) {
  int count = 0;

  auto finn = [&count]() {
    for (size_t i = 0; i < 10; ++i) {
      ++count;
      Yield();
      ASSERT_EQ(count, 0);
    }
    ++count;
  };

  auto jake = [&count]() {
    for (size_t i = 0; i < 10; ++i) {
      --count;
      Yield();
      ASSERT_EQ(count, 1);
    }
  };

  RunScheduler([&]() {
    Spawn(finn);
    Spawn(jake);
  });
}

TEST(Fibers, SleepFor) {
  RunScheduler([]() {
    tiny::support::StopWatch stop_watch;
    SleepFor(1s);
    ASSERT_GE(stop_watch.Elapsed(), 1s);
  });
}

TEST(Fibers, FifoScheduling) {
  static const size_t kFibers = 5;
  static const size_t kRounds = 5;

  size_t next = 0;

  auto routine = [&](size_t k) {
    for (size_t i = 0; i < kRounds; ++i) {
      ASSERT_EQ(next, k);
      next = (next + 1) % kFibers;
      Yield();
    }
  };

  RunScheduler([&]() {
    for (size_t k = 0; k < kFibers; ++k) {
      Spawn([&, k](){ routine(k); });
    }
  });
}

TEST(Fibers, WaitQueue) {
  WaitQueue wait_queue;
  int step = 0;

  auto foo = [&]() {
    ASSERT_EQ(step, 1);
    wait_queue.WakeOne();
    ASSERT_EQ(step, 1);
    ++step;
    Yield();
    ASSERT_EQ(step, 3);
  };

  auto main = [&]() {
    Spawn(foo);
    ++step;
    wait_queue.Park();
    ASSERT_EQ(step, 2);
    ++step;
    Yield();
  };

  RunScheduler(main);
}

TEST(Fibers, Mutex) {
  Mutex mutex;
  bool critical = false;

  auto routine = [&]() {
    for (size_t i = 0; i < 10; ++i) {
      mutex.Lock();
      ASSERT_FALSE(critical);
      critical = true;
      for (size_t j = 0; j < 3; ++j) {
        Yield();
      }
      ASSERT_TRUE(critical);
      critical = false;
      mutex.Unlock();
      Yield();
    }
  };

  RunScheduler([&]() {
    Spawn(routine);
    Spawn(routine);
  });
}

TEST(Fibers, MutexTryLock) {
  Mutex mutex;

  auto locker = [&mutex]() {
    mutex.Lock();
    Yield();
    mutex.Unlock();
  };

  auto try_locker = [&mutex]() {
    ASSERT_FALSE(mutex.TryLock());
    Yield();
    ASSERT_TRUE(mutex.TryLock());
    mutex.Unlock();
  };

  RunScheduler([&]() {
    Spawn(locker);
    Spawn(try_locker);
  });
}

TEST(Fibers, ConditionVariable) {
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
      Yield();
    }
    message = "Hello";
    ready.NotifyOne();
  };

  auto init = [&]() {
    Spawn(receive);
    Spawn(send);
  };

  RunScheduler(init);
}

class OnePassBarrier {
 public:
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

TEST(Fibers, Barrier) {
  OnePassBarrier barrier;
  size_t arrived = 0;

  static const size_t kFibers = 100;

  auto participant = [&]() {
    ++arrived;
    barrier.Arrive();
    ASSERT_EQ(arrived, kFibers);
  };

  RunScheduler([&]() {
    for (size_t i = 0; i < kFibers; ++i) {
      Spawn(participant);
    }
  });
}

TEST(Fibers, NoLeaks) {
  auto strong_ref = std::make_shared<int>(42);
  std::weak_ptr<int> weak_ref = strong_ref;

  RunScheduler([ref = std::move(strong_ref)]() {
    std::cout << "Answer to the Ultimate Question of Life, the Universe, and Everything: "
              << *ref << std::endl;
  });

  ASSERT_FALSE(weak_ref.lock());
}