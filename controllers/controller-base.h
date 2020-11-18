#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <memory>

#include "../actions/action.h"

namespace controllers {
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    virtual std::unique_ptr<actions::Action> action() = 0;
};
}

#endif
