#pragma once

#include <tf/rt/id.hpp>

namespace tf::rt {

class IdGenerator {
 public:
  FiberId Generate() {
    return ++last_id_;
  }

 private:
  size_t last_id_ = 0;
};

}  // namespace tf::rt
