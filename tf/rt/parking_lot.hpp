#pragma once

#include <tf/rt/fwd.hpp>

namespace tf::rt {

class ParkingLot {
 public:
  void Park();
  void Wake();

 private:
  Fiber* waitee_{nullptr};
};

}  // namespace tf::rt
