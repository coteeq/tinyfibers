#pragma once
#include "fwd.hpp"

namespace tf::rt {

class IPoller {
public:
    virtual ~IPoller() = default;
    virtual Fiber* TryPoll() = 0;
};

class NopPoller : public IPoller {
public:
    Fiber* TryPoll() override;
};

}
