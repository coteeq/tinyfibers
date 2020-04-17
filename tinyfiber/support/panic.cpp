#include <tinyfiber/support/panic.hpp>

#include <iostream>

namespace tiny::fiber {
namespace detail {

void Panic(const std::string& error) {
  std::cerr << error << std::endl;
  std::abort();
}

}  // namespace detail
}  // namespace tiny::fiber
