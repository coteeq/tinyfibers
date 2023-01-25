#pragma once

#include <tinyfibers/rt/fwd.hpp>

namespace tinyfibers::rt {

class ParkingLot {
 public:
  void Park();
  void Wake();

 private:
  Fiber* waitee_{nullptr};
};

}  // namespace tinyfibers::rt
