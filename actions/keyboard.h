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
    // DEL,
    TAB,
    ESC
};

class Keyboard final : public IAction<KeyType> {
private:
    char key;
public:
    Keyboard(KeyType value, char key = -1);
    
    char getKey();
    
    void execAction(models::TextModel &t) override;
    
    void undoAction(models::TextModel &t, int y, int x) override;
};
}

#endif
