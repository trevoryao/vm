#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <map>

#include "action.h"
#include "controller-base.h"

namespace controllers {
class Keyboard final : public ControllerBase {
private:
    std::map<char, Action> keyMap;
public:
    Action action() override;
};
}


#endif
