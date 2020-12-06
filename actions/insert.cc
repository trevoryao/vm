#include "insert.h"

#include <utility>

#include "movement.h"
#include "../models/text-model.h"

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
            t.moveAllCursor(y + getMult(), x);
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
            t.getMove().lastChar(newY, newX, 1, getMult());
            t.moveAllCursor(newY, newX + 1);
            break;
        }
        case InsType::CH_MVT: {
            int newY = y, newX = x;
            mvt->move(t, newY, newX);
            
            switch (mvt->getValue()) {
                case MvtType::LEFT: {
                    for (int i = 0; i < x - newX; ++i) t.getText().delChar(y, newX);
                    t.moveAllCursor(y, newX);
                    break;
                }
                case MvtType::RIGHT: {
                    for (int i = 0; i < newX - x; ++i) t.getText().delChar(y, x);
                    break;
                }
                case MvtType::UP: {
                    int j = getMult() == 1 ? 1 : 0;
                    for (int i = 0; i < y - newY + j; ++i) t.getText().delLine(y - 1);
                    t.getText().changeLine(newY);
                    t.moveAllCursor(newY, 0);
                    break;
                }
                case MvtType::DOWN: {
                    int j = getMult() == 1 ? 0 : 1;
                    t.getText().changeLine(y);
                    for (int i = 0; i < newY - y + j; ++i) t.getText().delLine(y + 1);
                    break;
                }
                case MvtType::WORD_LEFT: {
                    for (int i = 0; i < x - newX; ++i) t.getText().delChar(y, newX);
                    t.moveAllCursor(y, newX);
                    break;
                }
                case MvtType::WORD_RIGHT: {
                    for (int i = 0; i < newX - x; ++i) t.getText().delChar(y, x);
                    break;
                }
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: {
                    for (int i = 0; i < x - newX; ++i) t.getText().delChar(y, newX);
                    t.moveAllCursor(y, newX);
                    break;
                }
                case MvtType::END_CH: {
                    for (int i = 0; i <= newX - x; ++i) t.getText().delChar(y, x);
                    for (int i = 0; i < newY - y; ++i) t.getText().delLine(y + 1);
                    break;
                }
            }
            break;
        }
        case InsType::CH_LINE: {
            for (int i = 0; i < getMult() - 1; ++i) t.getText().delLine(y);
            t.getText().changeLine(y);
        } // delete line, insert empty line
        case InsType::CH: t.getText().delChar(y, x); break; // clear under char
    }
    t.clearStaticCmd();
    t.setInsertMode();
    t.displayAllViews();
}
}
