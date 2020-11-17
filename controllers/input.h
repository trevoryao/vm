#ifndef __INPUT_H__
#define __INPUT_H__

#include <map>
#include <memory>

#include "../actions/action-enums.h"
#include "controller-base.h"

class actions::Action;

namespace controllers {
class Input final : public ControllerBase {
private:
    std::map<int, ActionType> typeMap;
    std::map<char, MovementType> mvtMap;
    std::map<std::string, CommandType> cmdMap;
public:
    Input();
    
    std::unique_ptr<actions::Action> action() override;
};
}


#endif
