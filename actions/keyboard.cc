#include "keyboard.h"

#include <memory>
#include <utility>

#include "add.h"
#include "backspace.h"
#include "delete.h"
#include "../models/text-model.h"

using namespace std;
using namespace models;

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
            if (!t.getUndo().hasBuffer() || !t.getUndo().getBuffer()->canAdd(getValue()))
                t.getUndo().setBuffer(make_unique<Add>(y, x));
            else t.getUndo().getBuffer()->addEvent(-1, x);
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
            char c = t.getText().backSpace(y, x);
            t.moveAllCursor(newY, newX);
            if (c == -1) break;
            if (c == '\n') {
                t.getUndo().addRegister(Register{newY, newX, make_unique<Keyboard>(*this)});
                t.getUndo().setBuffer(make_unique<Add>(newY, newX));
            }
            // deal with new lines 
            else if (t.getUndo().hasBuffer() && t.getUndo().getBuffer()->canAdd(getValue())) {
                t.getUndo().getBuffer()->addEvent(c, newX);
            } else {
                t.getUndo().setBuffer(make_unique<Backspace>(c, y)); // add in ctor
            }
            break;
        }
        case KeyType::RETURN: {
            t.getText().newLine(y, x); 
            t.moveAllCursor(y + 1, 0);
            t.getUndo().addRegister(Register{y + 1, 0, make_unique<Keyboard>(*this)});
            t.getUndo().setBuffer(make_unique<Add>(y + 1, 0));
            break;
        }
        /*
        case KeyType::DEL: {
            char c = t.getText().del(y, x);
            if (c == -1) break;
            // deal with new lines
            if (c == '\n') {
                t.getUndo().addRegister(Register{y, 0, make_unique<Keyboard>(*this)});
                t.getUndo().setBuffer(make_unique<Add>(y, x));
            } else if (t.getUndo().hasBuffer() && t.getUndo().getBuffer()->canAdd(getValue())) {
                t.getUndo().getBuffer()->addEvent(c, x);
            } else {
                t.getUndo().setBuffer(make_unique<Delete>(c, y, x)); // add in ctor
            }
            break;
        }*/
        case KeyType::TAB: {
            t.getText().indent(y, x);
            t.moveAllCursor(y, x + 4);
            if (!t.getUndo().hasBuffer() || !t.getUndo().getBuffer()->canAdd(getValue()))
                t.getUndo().setBuffer(make_unique<Add>(y, x));
            break;
        }
        case KeyType::ESC: {
            t.setCmdMode();
            t.moveAllCursor(y, x > 0 ? x - 1 : 0);
            // if (t.getUndo().hasBuffer()) t.getUndo().getBuffer()->addEvent(-1, x);
            t.getUndo().addBuffer();
            break;
        }
    }
    t.displayAllViews();
}

void Keyboard::undoAction(TextModel &t, int y, int x) {
    switch (getValue()) {
        case KeyType::RETURN: {
            int newY = y - 1;
            int newX = t.getText().getTextFile()[y - 1].size() - 2;
            t.getText().backSpace(y, x);
            t.moveAllCursor(newY, newX);
            break;
        }
        case KeyType::BACKSPACE: t.getText().newLine(y, x); t.moveAllCursor(y + 1, 0); break;
        case KeyType::DEL: t.getText().newLine(y, x + 1); break;
        default: break;
    }
    t.displayAllViews();
}
}
