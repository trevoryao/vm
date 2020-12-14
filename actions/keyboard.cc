#include "keyboard.h"

#include <memory>
#include <utility>

#include "add.h"
#include "backspace.h"
#include "replace-buf.h"
#include "../models/mode-type.h"
#include "../models/text-model.h"
#include "../models/row.h"

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
            char c = 0;
            if (t.getMode() == ModeType::INSERT || 
                static_cast<size_t>(x) >= t.getText().getTextFile()[y].size() - 1) {
                t.getText().insert(key, y, x);
            } else {
                c = t.getText().replaceChar(key, y, x);
            }
            
            if (t.getText().isCpp() && t.getMode() == ModeType::INSERT) {
                switch (key) {
                    case '{': t.getText().insert('}', y, x + 1); break;
                    case '[': t.getText().insert(']', y, x + 1); break;
                    case '(': t.getText().insert(')', y, x + 1); break;
                    case '\'': t.getText().insert('\'', y, x + 1); break;
                    case '\"': t.getText().insert('\"', y, x + 1); break;
                }
            }
            if (!t.getUndo().hasBuffer() || !t.getUndo().getBuffer()->canAdd(getValue())) {
                if (t.getMode() == ModeType::INSERT || 
                    static_cast<size_t>(x) >= t.getText().getTextFile()[y].size() - 1)
                    t.getUndo().setBuffer(make_unique<Add>(key, y, x));
                else
                    t.getUndo().setBuffer(make_unique<ReplaceBuf>(key, c, y, x));
            }
            else t.getUndo().getBuffer()->addEvent(key, c, x);
            t.moveAllCursor(y, x + 1);
            break;
        }
        case KeyType::BACKSPACE: {
            if (x == 0 && y == 0) break;
            if (t.getMode() == ModeType::REPLACE) {
                if (x > 0) {
                    x = 0;
                } else {
                    y = y > 0 ? y - 1 : 0;
                    x = t.getText().getTextFile()[y].size() - 1;
                }
                break;
            }
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
                t.getUndo().setBuffer(make_unique<Add>("", newY, newX));
            }
            // deal with new lines 
            else if (t.getUndo().hasBuffer() && t.getUndo().getBuffer()->canAdd(getValue())) {
                t.getUndo().getBuffer()->addEvent(c, 0, newX);
            } else {
                t.getUndo().setBuffer(make_unique<Backspace>(c, y, x)); // add in ctor
            }
            break;
        }
        case KeyType::RETURN: {
            t.getText().newLine(y, x);
            t.moveAllCursor(y + 1, 0);
            t.getUndo().addRegister(Register{y + 1, 0, make_unique<Keyboard>(*this)});
            t.getUndo().setBuffer(make_unique<Add>("", y + 1, 0));
            break;
        }
        case KeyType::TAB: {
            t.getText().indent(y, x);
            t.moveAllCursor(y, x + 4);
            if (!t.getUndo().hasBuffer() || !t.getUndo().getBuffer()->canAdd(getValue()))
                t.getUndo().setBuffer(make_unique<Add>(string(INDENT_SIZE, ' '), y, x));
            else {
                for (int i = 0; i < INDENT_SIZE; ++i) t.getUndo().getBuffer()->addEvent(' ', 0, x + i);
            }
            break;
        }
        case KeyType::ESC: {
            t.setCmdMode();
            t.moveAllCursor(y, x > 0 ? x - 1 : 0);
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
        /* case KeyType::DEL: t.getText().newLine(y, x + 1); break; */
        default: break;
    }
    t.displayAllViews();
}
}
