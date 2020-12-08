#include "delete.h"

#include <vector>
#include <string>

#include "buffer.h"
#include "keyboard.h"
#include "../models/text-model.h"

namespace actions {
Delete::Delete(char c, int y, int x) : y{y}, x1{x}, buffer{c} { }

Delete::Delete(const std::string &s, int y, int x) : y{y}, x1{x} {
    for (auto &c : s) buffer.push_back(c);
}

void Delete::addEvent(char c, int) { 
    buffer.push_back(c);
}

void Delete::execAction(models::TextModel &t) { }

void Delete::undoAction(models::TextModel &t, int, int) {
    for (auto &c : buffer) 
        t.getText().insert(c, y, x1++);
    t.moveAllCursor(y, x1);
    t.displayAllViews();
}

bool Delete::canAdd(KeyType value) { return value == KeyType::DEL; }
}
