#include "movement.h"

#include "../models/text-model.h"

using namespace models;

namespace actions {
Movement::Movement(MvtType value, int n) : IAction{value, n} { }

void Movement::execAction(TextModel &t) {
    switch (getValue()) {
        case MvtType::LEFT: t.moveLeft(getMult()); break;
        case MvtType::RIGHT: t.moveRight(getMult()); break;
        case MvtType::UP: t.moveUp(getMult()); break;
        case MvtType::DOWN: t.moveDown(getMult()); break;
        case MvtType::WORD_LEFT: t.searchWordLeft(getMult()); break;
        case MvtType::WORD_RIGHT: t.searchWordRight(getMult()); break;
        case MvtType::BEG_LINE:
            int y, x;
            t.getCursor(y, x);
            t.moveLeft(x); break;
        case MvtType::BEG_CH: t.getFirstChar(); break;
        case MvtType::END_CH: t.getLastChar(getMult()); break;
    }
}
}
