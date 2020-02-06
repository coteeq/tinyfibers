#include "gtest/gtest.h"

#include <clew/core/api.hpp>
#include <clew/sync/mutex.hpp>
#include <clew/sync/condvar.hpp>

using namespace clew::fiber;

TEST(Fibers, YieldOnce) {
  auto routine = []() {
    Yield();
  };
  RunScheduler(routine);
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
  size_t step = 0;

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

TEST(Fibers, MutualExclusion) {
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