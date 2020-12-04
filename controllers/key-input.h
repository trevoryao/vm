#ifndef __KEY_INPUT_H__
#define __KEY_INPUT_H__

#include <map>
#include <memory>

#include "controller-base.h"
#include "../actions/action.h"
#include "../actions/keyboard.h"
#include "../actions/movement.h"

using actions::Action;

namespace actions {
class Incomplete;
class ESearch;
class Search;
}

namespace controllers {
class KeyInput final : public ControllerBase {
private:
    enum class ActionType { KEY, MVT };
    std::map<int, ActionType> actionMap;
    std::map<int, actions::KeyType> keyMap;
    std::map<int, actions::MvtType> mvtMap;
public:
    KeyInput();
protected:
    std::unique_ptr<Action> action() override;
    std::unique_ptr<Action> action(actions::Incomplete *a) override;
    std::unique_ptr<Action> action(actions::ESearch *a) override;
    std::unique_ptr<Action> action(actions::Search *a) override;
};
}

#endif
