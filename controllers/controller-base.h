#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include "action.h"

namespace controllers {
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    virtual Action action() = 0;
};
}

#endif
