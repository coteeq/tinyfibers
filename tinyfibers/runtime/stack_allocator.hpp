#pragma once

#include <tinyfibers/context/stack.hpp>

namespace tiny::fibers {

context::Stack AllocateStack();
void ReleaseStack(context::Stack stack);

}  // namespace tiny::fibers
