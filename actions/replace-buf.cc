#include "replace-buf.h"

#include <string>

#include "buffer.h"
#include "keyboard.h"
#include "../models/text-model.h"

using namespace models;

namespace actions {
ReplaceBuf::ReplaceBuf(char r, char c, int y, int x) :
    y{y}, x1{x}, original(1, c), replacement(1, r) { }

void ReplaceBuf::addEvent(char r, char c, int x) {
    original.push_back(c);
    replacement.push_back(r);
    x2 = x;
}

bool ReplaceBuf::canAdd(KeyType value) { return value == KeyType::ALPHA_NUM; }

void ReplaceBuf::execAction(TextModel &t) {
    int curY, curX;
    t.getCursor(curY, curX);
    size_t i = 0;
    for (; i < replacement.size(); ++i) {
        t.getText().replaceChar(replacement[i], curY, curX + i);
    }
    t.moveAllCursor(curY, curX + i);
    t.displayAllViews();
}

void ReplaceBuf::undoAction(TextModel &t, int, int) {
    for (size_t i = 0; i < original.size(); ++i) {
        t.getText().replaceChar(original[i], y, x1 + i);
    }
    t.moveAllCursor(y, x2);
    t.displayAllViews();
}
}
