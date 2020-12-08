#include "backspace.h"

#include <list>
#include <string>

#include "buffer.h"
#include "keyboard.h"
#include "../models/text-model.h"

namespace actions {
Backspace::Backspace(char c, int y) : y{y}, buffer{c} { }

Backspace::Backspace(const std::string &s, int y) : y{y} {
    for (auto &c : s) buffer.push_back(c);
}

void Backspace::addEvent(char c, int x) { 
    buffer.push_front(c);
    x2 = x; 
}

void Backspace::execAction(models::TextModel &t) { }

void Backspace::undoAction(models::TextModel &t, int, int) {
    for (auto &c : buffer) t.getText().insert(c, y, x2++);
    t.moveAllCursor(y, x2);
    t.displayAllViews();
}

bool Backspace::canAdd(KeyType value) { return value == KeyType::BACKSPACE; }
}
