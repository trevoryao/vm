#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

namespace controllers {
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    virtual void action() = 0;
};
}

#endif
