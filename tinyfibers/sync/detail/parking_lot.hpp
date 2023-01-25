#pragma once

#include <tinyfibers/rt/fwd.hpp>

namespace tinyfibers {

namespace detail {

class ParkingLot {
 public:
  void Park();
  void Wake();

 private:
  Fiber* waitee_{nullptr};
};

}  // namespace detail

}  // namespace tinyfibers
