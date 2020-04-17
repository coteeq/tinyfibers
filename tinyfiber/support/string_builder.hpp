#pragma once

#include <sstream>

namespace tiny::fiber {

class StringBuilder {
 public:
  template <typename T>
  StringBuilder& operator<<(const T& next) {
    out_ << next;
    return *this;
  }

  operator std::string() const {
    return out_.str();
  }

  std::string String() const {
    return out_.str();
  }

 private:
  std::ostringstream out_;
};

}  // namespace tiny::fiber
