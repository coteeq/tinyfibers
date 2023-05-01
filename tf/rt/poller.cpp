#include "poller.hpp"

namespace tf::rt {

Fiber* NopPoller::TryPoll() {
    return nullptr;
}

}
