#pragma once

#include <tf/rt/fwd.hpp>

#include <function2/function2.hpp>

namespace tf::rt {

// System call handler
using Handler = fu2::function_view<void(Fiber*)>;

}  // namespace tf::rt
