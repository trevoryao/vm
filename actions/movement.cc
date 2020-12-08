#include "movement.h"

#include "../models/text-model.h"

using namespace models;

namespace actions {
Movement::Movement(MvtType value, int n) : IAction{value, n} { }

void Movement::move(TextModel &t, int &y, int &x) {
    switch (getValue()) {
        case MvtType::LEFT: t.getMove().left(y, x, getMult()); break;
        case MvtType::RIGHT: t.getMove().right(y, x, getMult(), t.getMode()); break;
        case MvtType::UP: t.getMove().up(y, x, getMult(), t.getMode()); break;
        case MvtType::DOWN: t.getMove().down(y, x, getMult(), t.getMode()); break;
        case MvtType::WORD_LEFT: t.getMove().wordBwd(y, x, getMult()); break;
        case MvtType::WORD_RIGHT: t.getMove().wordFwd(y, x, getMult()); break;
        case MvtType::BEG_LINE: t.getMove().begLine(y, x); break;
        case MvtType::BEG_CH: t.getMove().firstChar(y, x); break;
        case MvtType::END_CH: t.getMove().lastChar(y, x, getMult()); break;
    }
}

void Movement::execAction(TextModel &t) {
    int y, x;
    t.getCursor(y, x);
    move(t, y, x);
    t.moveAllCursor(y, x);
    t.getUndo().addBuffer();
    t.clearStaticCmd();
}
}
