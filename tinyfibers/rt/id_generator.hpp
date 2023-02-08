#pragma once

#include <tinyfibers/rt/id.hpp>

namespace tinyfibers::rt {

class IdGenerator {
 public:
  FiberId Generate() {
    return ++last_id_;
  }

 private:
  size_t last_id_ = 0;
};

}  // namespace tinyfibers::rt
