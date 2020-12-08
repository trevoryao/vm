#include "insert.h"

#include <memory>
#include <utility>

#include "movement.h"
#include "../models/text-model.h"
#include "../models/register.h"

using namespace std;
using namespace models;

namespace actions {
Insert::Insert(InsType value, int n, std::unique_ptr<Movement> mvt) : 
    IAction{value, n}, mvt{std::move(mvt)} { }

void Insert::execAction(TextModel &t) {
    int y, x;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case InsType::BEFORE: t.moveAllCursor(y, x - getMult() + 1); break;
        case InsType::AFTER: t.moveAllCursor(y, x + getMult()); break;
        case InsType::NEW_LINE_ABOVE: {
            for (int i = 0; i < getMult(); ++i) t.getText().newLine(y - 1);
            t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
            break;
        }
        case InsType::NEW_LINE_BELOW: {
            for (int i = 0; i < getMult(); ++i) t.getText().newLine(y);
            t.moveAllCursor(y + getMult(), x);
            t.getUndo().addRegister(Register{y + getMult(), x, move(make_unique<Insert>(move(*this)))});
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
                case MvtType::LEFT:
                case MvtType::WORD_LEFT:
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: {
                    std::string s;
                    for (int i = 0; i < x - newX; ++i) s.push_back(t.getText().delChar(y, newX));
                    t.moveAllCursor(y, newX);
                    rows.emplace_back(s, t.getText().getWidth());
                    t.getUndo().addRegister(Register{y, newX, move(make_unique<Insert>(move(*this)))});
                    break;
                }
                case MvtType::RIGHT:
                case MvtType::WORD_RIGHT: {
                    std::string s;
                    for (int i = 0; i < newX - x; ++i) s.push_back(t.getText().delChar(y, x));
                    rows.emplace_back(s, t.getText().getWidth());
                    t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
                    break;
                }
                case MvtType::UP: { // go backwards
                    int j = getMult() == 1 ? 0 : -1;
                    for (int i = 0; i < y - newY + j; ++i) 
                        rows.emplace_back(t.getText().delLine(newY));
                    rows.emplace_back(t.getText().changeLine(newY));
                    t.moveAllCursor(newY, 0);
                    t.getUndo().addRegister(Register{newY, x, move(make_unique<Insert>(move(*this)))});
                    break;
                }
                case MvtType::DOWN: {
                    int j = getMult() == 1 ? 0 : 1;
                    rows.emplace_back(t.getText().changeLine(y));
                    for (int i = 0; i < newY - y + j; ++i) rows.emplace_back(t.getText().delLine(y + 1));
                    t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
                    break;
                }
                case MvtType::END_CH: {
                    std::string s;
                    if (mvt->getMult() == 1) {
                        for (int i = 0; i <= newX - x; ++i) {
                            s.push_back(t.getText().delChar(y, x));
                            t.displayAllViews();
                        }
                        rows.emplace_back(s, t.getText().getWidth());
                        t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
                        break;
                    }
                    size_t size = t.getText().getTextFile()[y].size();
                    for (size_t i = x; i < size; ++i) {
                        s.push_back(t.getText().delChar(y, x));
                        t.displayAllViews();
                    }
                    rows.emplace_back(s, t.getText().getWidth());
                    s.clear();
                    for (int i = 0; i <= newX; ++i) {
                        s.push_back(t.getText().delChar(newY, 0));
                        t.displayAllViews();
                    }
                    for (int i = 0; i < newY - y - 1; ++i) {
                        rows.emplace_back(t.getText().delLine(y + 1));
                        t.displayAllViews();
                    }
                    rows.emplace_back(s, t.getText().getWidth());
                    t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
                    break;
                }
            }
            break;
        }
        case InsType::CH_LINE: {
            for (int i = 0; i < getMult() - 1; ++i) rows.emplace_back(t.getText().delLine(y));
            rows.emplace_back(t.getText().changeLine(y));
            t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
            break;
        } // delete line, insert empty line
        case InsType::CH: {
            std::string s;
            for (int i = 0; i < getMult(); ++i) s.push_back(t.getText().delChar(y, x));
            rows.emplace_back(s, t.getText().getWidth());
            t.getUndo().addRegister(Register{y, x, move(make_unique<Insert>(move(*this)))});
            break; // clear under char
        }
    }
    t.clearStaticCmd();
    t.setInsertMode();
    t.displayAllViews();
}

void Insert::undoAction(TextModel &t, int y, int x) {
    switch (getValue()) {
        case InsType::NEW_LINE_ABOVE: {
            for (int i = 0; i < getMult(); ++i) t.getText().delLine(y);
            break;
        }
        case InsType::NEW_LINE_BELOW: {
            for (int i = 0; i < getMult(); ++i) t.getText().delLine(y - getMult() + 1);
            t.moveAllCursor(y - getMult(), x);
            break;
        }
        case InsType::CH_MVT: {
            switch (mvt->getValue()) {
                case MvtType::LEFT:
                case MvtType::WORD_LEFT:
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: {
                    t.getText().insert(rows.front(), y, x);
                    t.moveAllCursor(y, x + rows.front().size());
                    break;
                }
                case MvtType::RIGHT:
                case MvtType::WORD_RIGHT: {
                    t.getText().insert(rows.front(), y, x);
                    break;
                }
                case MvtType::UP: { // go backwards
                    t.getText().replaceLine(rows.back(), y);
                    for (size_t i = rows.size(); i != 0; --i)
                        t.getText().insert(rows[i - 1], y - 1);
                    t.moveAllCursor(y + rows.size(), x);
                    break;
                }
                case MvtType::DOWN: {
                    t.getText().replaceLine(rows.front(), y);
                    for (size_t i = rows.size(); i != 0; --i) 
                        t.getText().insert(rows[i - 1], y);
                    break;
                }
                case MvtType::END_CH: {
                    t.getText().insert(rows.front(), y, x);
                    if (rows.size() == 1) break;
                    for (size_t i = 1; i < rows.size() - 1; ++i) 
                        t.getText().insert(rows[i], y);
                    t.getText().insert(rows.back(), y + rows.size() - 1, 0);
                    break;
                }
            }
            break;
        }
        case InsType::CH_LINE: {
            t.getText().replaceLine(rows.back(), y);
            for (size_t i = rows.size() - 1; i != 0; --i)
                t.getText().insert(rows[i - 1], y - 1);
            break;
        }
        case InsType::CH: {
            t.getText().insert(rows.front(), y, x);
            break;
        }
        default: break;
    }
    t.displayAllViews();
}
}
