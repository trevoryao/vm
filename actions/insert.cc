#include "insert.h"

#include <utility>

#include "movement.h"
#include "../models/text-model.h"

namespace {
// retval > 0 if vec1 > vec2, 0 if equal, -1 if vec1 < vec2
int coordCmp(int y1, int x1, int y2, int x2) {
    if (y1 != y2) return y1 - y2;
    // y1 == y2
    return x1 - x2;
}
}

namespace actions {
Insert::Insert(InsType value, int n, std::unique_ptr<Movement> mvt) : 
    IAction{value, n}, mvt{std::move(mvt)} { }

void Insert::execAction(models::TextModel &t) {
    int y, x;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case InsType::BEFORE: t.moveAllCursor(y, x - getMult() + 1); break;
        case InsType::AFTER: t.moveAllCursor(y, x + getMult()); break;
        case InsType::NEW_LINE_ABOVE: {
            for (int i = 0; i < getMult(); ++i) t.getText().newLine(y - 1);
            break;
        }
        case InsType::NEW_LINE_BELOW: {
            for (int i = 0; i < getMult(); ++i) t.getText().newLine(y);
            break;
        }
        case InsType::BEG_LINE: {
            int newY = y, newX = x;
            t.getMove().firstChar(newY, newX, getMult());
            t.moveAllCursor(newY, newX);
            break;
        }
        case InsType::END_LINE: {
            int newY = y, newX = x;
            t.getMove().lastChar(y, x, 1, getMult());
            t.moveAllCursor(newY, newX);
            break;
        }
        case InsType::CH_MVT: {
            int newY = y, newX = x;
            mvt->move(t, y, x);
            if (coordCmp(y, x, newY, newX) > 0) {
                std::swap(y, newY);
                std::swap(x, newX);
            }
            // (y, x) <= (newY, newX)
            if (y != newY) {
                for (int i = 0; i < mvt->getMult() - 1; ++i) t.getText().delLine(y);
                t.getText().changeLine(y);
            } else {
                for (int i = 0; i <= newX - x; ++i) t.getText().delChar(y, x);
            }
            break;
        } // delete from current to movement result
        case InsType::CH_LINE: {
            for (int i = 0; i < getMult() - 1; ++i) t.getText().delLine(y);
            t.getText().changeLine(y);
        } // delete line, insert empty line
        case InsType::CH: t.getText().delChar(y, x); break; // clear under char
    }
    
    t.displayViews();
    t.setInsertMode();
}
}
