#include "test.hpp"

#include <tf/rt/run.hpp>

#include <tf/sched/spawn.hpp>
#include <tf/sched/yield.hpp>
#include <tf/sched/sleep_for.hpp>
#include <tf/sched/id.hpp>
#include <tf/sync/wait_group.hpp>
#include <tf/sync/mutex.hpp>
#include <tf/sync/condvar.hpp>

#include <wheels/core/stop_watch.hpp>

#include <memory>
#include <chrono>
#include <type_traits>

using namespace tf;

using namespace std::chrono_literals;

TEST_SUITE(Fibers) {
  SIMPLE_TEST(JustWorks) {
    rt::RunScheduler([]() {
      Yield();
    });
  }

  // Runs test routine in fiber scheduler
  TINY_FIBERS_TEST(TestMacro) {
    Yield();
  }

  TINY_FIBERS_TEST(Join) {
    bool done = false;
    JoinHandle h = Spawn([&]() {
      done = true;
    });
    ASSERT_FALSE(done);
    h.Join();
    ASSERT_TRUE(done);
  }

  TINY_FIBERS_TEST(JoinCompleted) {
    bool done = false;
    JoinHandle h = Spawn([&]() {
      done = true;
    });
    Yield();
    ASSERT_TRUE(done);
    h.Join();
  }

  // At least does not panic
  TINY_FIBERS_TEST(Detach) {
    JoinHandle h = Spawn([&]() {
      Yield();
    });
    h.Detach();
  }

  TINY_FIBERS_TEST(MoveJoinHandle) {
    JoinHandle h = Spawn([]() {});
    Yield();
    JoinHandle g{std::move(h)};
    g.Join();
  }

  TINY_FIBERS_TEST(Ids) {
    FiberId init_id = GetId();

    auto first = [&]() {
      ASSERT_EQ(GetId(), init_id + 1);
    };

    auto second = [&]() {
      ASSERT_EQ(GetId(), init_id + 2);
    };

    JoinHandle f1 = Spawn(first);
    JoinHandle f2 = Spawn(second);

    Yield();

    ASSERT_EQ(init_id, GetId());

    f1.Join();
    f2.Join();
  }

  TINY_FIBERS_TEST(PingPong) {
    int count = 0;

    auto first = [&count]() {
      for (size_t i = 0; i < 10; ++i) {
        ++count;
        Yield();
        ASSERT_EQ(count, 0);
      }
      ++count;
    };

    auto second = [&count]() {
      for (size_t i = 0; i < 10; ++i) {
        --count;
        Yield();
        ASSERT_EQ(count, 1);
      }
    };

    WaitGroup wg;
    wg.Spawn(first).Spawn(second);
    wg.Wait();
  }

  TINY_FIBERS_TEST(SleepFor) {
    wheels::StopWatch stop_watch;
    SleepFor(1s);
    ASSERT_GE(stop_watch.Elapsed(), 1s);
  }

  TINY_FIBERS_TEST(FifoScheduling) {
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

    WaitGroup wg;
    for (size_t k = 0; k < kFibers; ++k) {
      wg.Spawn([&, k]() {
        routine(k);
      });
    }
    wg.Wait();
  }

  TINY_FIBERS_TEST(WaitQueue) {
    rt::WaitQueue wait_queue;
    int step = 0;

    JoinHandle waker = Spawn([&]() {
      ASSERT_EQ(step, 1);
      wait_queue.WakeOne();
      ASSERT_EQ(step, 1);
      ++step;
      Yield();
      ASSERT_EQ(step, 3);
    });

    ++step;
    wait_queue.Park();
    ASSERT_EQ(step, 2);
    ++step;
    Yield();
    waker.Join();
  }

  TINY_FIBERS_TEST(Mutex) {
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

    WaitGroup wg;
    wg.Spawn(routine).Spawn(routine);
    wg.Wait();
  }

  TINY_FIBERS_TEST(MutexTryLock) {
    Mutex mutex;

    WaitGroup wg;

    wg.Spawn([&mutex]() {
      mutex.Lock();
      Yield();
      mutex.Unlock();
    });

    wg.Spawn([&mutex]() {
      ASSERT_FALSE(mutex.TryLock());
      Yield();
      ASSERT_TRUE(mutex.TryLock());
      mutex.Unlock();
    });

    wg.Wait();
  }

  /*
  TINY_FIBERS_TEST(DoNotMoveMutex) {
    Mutex m1;
    Mutex m2{std::move(m1)};
  }
  */

  TINY_FIBERS_TEST(ConditionVariable) {
    Mutex mutex;
    CondVar ready;
    std::string message;

    WaitGroup wg;

    wg.Spawn([&]() {
      std::unique_lock lock(mutex);

      ready.Wait(mutex);
      ASSERT_EQ(message, "Hello");
    });

    wg.Spawn([&]() {
      std::lock_guard guard(mutex);

      for (size_t i = 0; i < 100; ++i) {
        Yield();
      }
      message = "Hello";
      ready.NotifyOne();
    });

    wg.Wait();
  }

  class OnePassBarrier {
   public:
    OnePassBarrier(size_t threads) : threads_(threads) {
    }

    void Arrive() {
      std::unique_lock lock(mutex_);
      --threads_;
      if (threads_ == 0) {
        all_arrived_.NotifyAll();
      } else {
        all_arrived_.Wait(lock, [this]() {
          return threads_ == 0;
        });
      }
    }

   private:
    size_t threads_;
    Mutex mutex_;
    CondVar all_arrived_;
  };

  TINY_FIBERS_TEST(Barrier) {
    static const size_t kFibers = 100;

    OnePassBarrier barrier{kFibers};
    size_t arrived = 0;

    WaitGroup wg;
    for (size_t i = 0; i < kFibers; ++i) {
      wg.Spawn([&]() {
        ++arrived;
        barrier.Arrive();
        ASSERT_EQ(arrived, kFibers);
      });
    }
    wg.Wait();
  }

  TINY_FIBERS_TEST(NonCopyable) {
    static_assert(!std::is_copy_assignable<Mutex>::value, "Broken Mutex");
    static_assert(!std::is_copy_constructible<Mutex>::value, "Broken Mutex");

    static_assert(!std::is_copy_assignable<CondVar>::value, "Broken CondVar");
    static_assert(!std::is_copy_constructible<CondVar>::value, "Broken CondVar");

    static_assert(!std::is_copy_assignable<rt::WaitQueue>::value, "Broken WaitQueue");
    static_assert(!std::is_copy_constructible<rt::WaitQueue>::value, "Broken WaitQueue");
  }

  SIMPLE_TEST(NoLeaks) {
    auto strong_ref = std::make_shared<int>(42);
    std::weak_ptr<int> weak_ref = strong_ref;

    rt::RunScheduler([ref = std::move(strong_ref)]() {
      std::cout << "Answer to the Ultimate Question of Life, the Universe, and "
                   "Everything: "
                << *ref << std::endl;
    });

    ASSERT_FALSE(weak_ref.lock());
  }
}
