#include "e-movement.h"

#include "../models/text-model.h"

namespace actions {
EMovement::EMovement(EMvtType value, int lineNum) : 
    IAction{value, 1}, lineNum{lineNum} { }

void EMovement::execAction(models::TextModel &t) {
    int y = 0, x = 0;
    t.getCursor(y, x);

    switch (getValue()) {
        case EMvtType::TOP: t.moveAllCursor(0, 0); break;
        case EMvtType::BOTTOM: t.moveAllCursor(t.getText().getTextFile().size() - 1, 0); break;
        case EMvtType::LINE_NUM: {
            if (static_cast<size_t>(lineNum) < t.getText().getTextFile().size()) t.moveAllCursor(lineNum - 1, 0);
            else t.moveAllCursor(t.getText().getTextFile().size() - 1, 0);
            break;
        }
    }
}
}
