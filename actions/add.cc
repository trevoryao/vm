#include "add.h"

#include <vector>
#include <utility>

#include "buffer.h"
#include "keyboard.h"
#include "../models/text-model.h"

using namespace std;

namespace actions {
Add::Add(int y, int x) : y{y}, x1{x} { }

void Add::addEvent(char, int x) { x2 = x; }

bool Add::canAdd(KeyType value) { return value == KeyType::ALPHA_NUM; }

void Add::execAction(models::TextModel &t) { }

void Add::undoAction(models::TextModel &t, int, int) {
    if (x1 == x2) return;
    for (int i = 0; i <= x2 - x1; ++i) {
        t.getText().delChar(y, x1);
    }
    t.moveAllCursor(y, x1);
    t.displayAllViews();
}
}
