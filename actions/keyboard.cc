#include "keyboard.h"

#include "../models/text-model.h"

namespace actions {
Keyboard::Keyboard(KeyType value, char key) : IAction{value}, key{key} { }

char Keyboard::getKey() { return key; }

void Keyboard::execAction(models::TextModel &t) {
    int y = 0, x = 0;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case KeyType::ALPHA_NUM: {
            t.getText().insert(key, y, x);
            t.moveAllCursor(y, x + 1);
            break;
        }
        case KeyType::BACKSPACE: {
            t.getText().delChar(y, x > 0 ? x - 1 : 0);
            t.moveAllCursor(y, x > 0 ? x - 1 : 0);
            break;
        }
        case KeyType::RETURN: t.getText().newLine(y, x); break;
        case KeyType::DEL: t.getText().delChar(y, x); break;
        case KeyType::TAB: {
            t.getText().indent(y, x);
            t.moveAllCursor(y, x + 4);
            break;
        }
        case KeyType::ESC: t.setCmdMode(); break;
    }
    t.displayViews();
}
}
