#ifndef __ACTION_H__
#define __ACTION_H__

#include "action-enums.h"

namespace actions {
class Action {
private:
    ActionType type;
public:
    Action(ActionType type = ActionType::NONE);
    
    ActionType getType();
    
    virtual ~Action() = default;
};
}

#endif
