#include <clew/support/panic.hpp>

#include <iostream>

namespace clew {
namespace detail {

void Panic(const std::string& error) {
  std::cerr << error << std::endl;
  std::abort();
}

}  // namespace detail
}  // namespace clew
