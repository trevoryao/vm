#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <memory>

class actions::Action;

namespace controllers {
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    virtual std::unique_ptr<actions::Action> action() = 0;
};
}

#endif
