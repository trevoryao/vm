#include "add.h"

#include <string>
#include <utility>

#include "buffer.h"
#include "keyboard.h"
#include "../models/row.h"
#include "../models/text-model.h"

using namespace std;

namespace actions {
Add::Add(char c, int y, int x) : y{y}, x1{x}, buffer(1, c) { }

Add::Add(const std::string &s, int y, int x) : y{y}, x1{x}, buffer{s} { }

void Add::addEvent(char c, int x) { 
    buffer.push_back(c);
    x2 = x; 
}

bool Add::canAdd(KeyType value) { return value == KeyType::ALPHA_NUM; }

void Add::execAction(models::TextModel &t) {
    int curY, curX;
    t.getCursor(curY, curX);
    t.getText().insert(models::Row{buffer, t.getText().getWidth()}, curY, curX);
    t.moveAllCursor(curY, curX + buffer.size());
}

void Add::undoAction(models::TextModel &t, int, int) {
    if (x1 == x2) return;
    for (int i = 0; i <= x2 - x1; ++i) {
        t.getText().delChar(y, x1);
    }
    t.moveAllCursor(y, x1);
    t.displayAllViews();
}
}
