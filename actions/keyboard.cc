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
            int newY, newX;
            if (x == 0) {
                newY = y - 1;
                newX = t.getText().getTextFile()[y - 1].size() - 1;
            } else {
                newY = y;
                newX = x - 1;
            }
            t.getText().backSpace(y, x);
            t.moveAllCursor(newY, newX);
            break;
        }
        case KeyType::RETURN: t.getText().newLine(y, x); t.moveAllCursor(y + 1, 0); break;
        case KeyType::DEL: {
            t.getText().del(y, x);
            break;
        }
        case KeyType::TAB: {
            t.getText().indent(y, x);
            t.moveAllCursor(y, x + 4);
            break;
        }
        case KeyType::ESC: t.setCmdMode(); t.moveAllCursor(y, x > 0 ? x - 1 : 0); break;
    }
    t.displayAllViews();
}
}
