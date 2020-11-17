#ifndef __INPUT_H__
#define __INPUT_H__

#include <map>

#include "action.h"
#include "controller-base.h"

namespace controllers {
class Input final : public ControllerBase {
private:
    std::map<char, Action> keyMap;
public:
    Action action() override;
};
}


#endif
