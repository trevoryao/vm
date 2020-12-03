#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class KeyType {
    ALPHA_NUM,
    BACKSPACE,
    RETURN,
    DEL,
    TAB,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Keyboard final : IAction<KeyType> {
private:
    char key;
public:
    Keyboard(KeyType value, char key);
    
    char getKey();
    
    void execAction(models::TextModel &t) { }
};
}

#endif
