#ifndef __INPUT_H__
#define __INPUT_H__

#include <map>
#include <memory>

#include "../actions/action-enums.h"
#include "../actions/action.h"
#include "controller-base.h"

namespace controllers {
class Input final : public ControllerBase {
private:
    std::map<int, actions::ActionType> typeMap;
    std::map<char, actions::MovementType> mvtMap;
    std::map<std::string, actions::CommandType> cmdMap;
public:
    Input();
    
    std::unique_ptr<actions::Action> action() override;
};
}


#endif
