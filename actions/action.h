#ifndef __ACTION_BASE_H__
#define __ACTION_BASE_H__

namespace actions {
enum class ActionType {
    E_MVT,
    E_SEARCH,
    FILE_OP,
    GLOBAL,
    INCOMPLETE,
    INS,
    KEYBOARD,
    MVT,
    REPLACE,
    SCROLL,
    SEARCH,
    TEXT_EDIT
};

class Action {
private:
    ActionType type;
    int mult;
public:
    Action(ActionType type, int n);
    
    virtual ~Action() = default;
    
    ActionType getType();
    int getMult();
};
}

#endif
