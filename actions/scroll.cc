#include "scroll.h"

#include "../models/text-model.h"

namespace actions {
Scroll::Scroll(ScrollType value) : IAction{value,
    (value == ScrollType::PG_FWD || value == ScrollType::PG_BACK) ? 1 : -1} { }

Scroll::Scroll(ScrollType value, int n) : IAction{value, n} { }

// -1 = 1/2
void Scroll::execAction(models::TextModel &t) {
    int winSize = t.getHeight() - 1;
    switch (getValue()) {
        case ScrollType::PG_FWD:
            t.getText().scrollDown(getMult() * winSize);
            break;
        case ScrollType::PG_BACK:
            t.getText().scrollUp(getMult() * winSize);
            break;
        case ScrollType::LINE_FWD:
            t.getText().scrollDown(getMult() == -1 ? winSize / 2 : getMult());
            break;
        case ScrollType::LINE_BACK:
            t.getText().scrollUp(getMult() == -1 ? winSize / 2 : getMult());
            break;
    }
    t.clearStaticCmd();
    t.moveAllCursor(t.getText().getTopLine(), 0);
    t.displayAllViews();
}
}
