#pragma once

#include <chrono>

namespace clew {

////////////////////////////////////////////////////////////////////////////////

using Duration = std::chrono::nanoseconds;

////////////////////////////////////////////////////////////////////////////////

class Timer {
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;

 public:
  Timer() : start_time_(Now()) {
  }

  Duration Elapsed() const {
    return Now() - start_time_;
  }

  Duration Restart() {
    auto elapsed = Elapsed();
    start_time_ = Now();
    return elapsed;
  }

 private:
  static TimePoint Now() {
    return Clock::now();
  }

 private:
  TimePoint start_time_;
};

}  // namespace clew
