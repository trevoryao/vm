#include "backspace.h"

#include <list>
#include <string>

#include "buffer.h"
#include "keyboard.h"
#include "../models/text-model.h"

namespace actions {
Backspace::Backspace(char c, int y, int x1) :  y{y}, x1{x1}, buffer{c} { }

Backspace::Backspace(const std::string &s, int y, int x1) : y{y}, x1{x1} {
    for (auto &c : s) buffer.push_back(c);
}

void Backspace::addEvent(char c, int x) { 
    buffer.push_front(c);
    x2 = x; 
}

void Backspace::execAction(models::TextModel &t) {
    int curY, curX;
    t.getCursor(curY, curX);
    int diff = x1 - x2;
    int newX = curX - diff < 0 ? 0 : curX - diff;
    for (int i = 0; i <= x1 - x2; ++i) {
        if (static_cast<size_t>(curX) >= t.getText().getTextFile()[y].size()) break;
        t.getText().delChar(curY, newX);
    }
    t.moveAllCursor(curY, newX);
}

void Backspace::undoAction(models::TextModel &t, int, int) {
    int i = x2;
    for (auto &c : buffer) t.getText().insert(c, y, i++);
    t.moveAllCursor(y, i);
    t.displayAllViews();
}

bool Backspace::canAdd(KeyType value) { return value == KeyType::BACKSPACE; }
}
