#include "text-edit.h"

#include <memory>
#include <string>
#include <utility>

#include "../exceptions/display-message.h"
#include "../models/text-model.h"

using namespace std;
using namespace models;

namespace actions {
TextEdit::TextEdit(TextEditType value, int n, std::unique_ptr<Movement> mvt) :
    IAction{value, n}, mvt{std::move(mvt)} { }

void TextEdit::execAction(models::TextModel &t) {
    int y, x;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case TextEditType::DEL_LINE: {
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(y) >= t.getText().getTextFile().size()) break;
                rows.push_back(t.getText().delLine(y));
            }
            t.getClipboard().add(move(clone()));
            t.getUndo().addRegister(Register{y, x, clone()});
            break;
        }
        case TextEditType::DEL_MVT: delMvt(t, y, x); break;
        case TextEditType::DEL_UNDER: {
            std::string s;
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(x) >= t.getText().getTextFile()[y].size() - 1) break;
                s.push_back(t.getText().delChar(y, x));
            }
            rows.emplace_back(s, t.getText().getWidth());
            t.getClipboard().add(move(clone()));
            t.getUndo().addRegister(Register{y, x, clone()});
            break;
        }
        case TextEditType::DEL_BEFORE: {
            std::string s;
            if (x - getMult() >= 0) {
                for (int i = 0; i < getMult(); ++i) 
                    s.push_back(t.getText().delChar(y, x - getMult()));
                t.moveAllCursor(y, x - getMult());
                
            } else {
                for (int i = 0; i < x; ++i) 
                    s.push_back(t.getText().delChar(y, 0));
                t.moveAllCursor(y, 0);
            }
            rows.emplace_back(s, t.getText().getWidth());
            t.getClipboard().add(move(clone()));
            t.getUndo().addRegister(Register{y, x - getMult() < 0 ? 0 : x - getMult(), clone()});
            break;
        }
        case TextEditType::PUT_BEFORE: if (t.getClipboard()) putBefore(t, y, x); break;
        case TextEditType::PUT_AFTER: if (t.getClipboard()) putAfter(t, y, x); break;
        case TextEditType::UNDO: t.getUndo().undo(t); break;
        case TextEditType::REPEAT: t.getUndo().execLastRegister(t); break;
        case TextEditType::YANK_LINE: {
            for (int i = 0; i < getMult(); ++i)  {
                if (static_cast<size_t>(y + i) >= t.getText().getTextFile().size()) break;
                rows.push_back(t.getText().getTextFile()[y + i]);
            }
            t.getClipboard().add(move(clone()));
            break;
        }
        case TextEditType::YANK_MVT: yankMvt(t, y, x); break;
        case TextEditType::JOIN: {
            posns.push_back(t.getText().getTextFile()[y].size() - 1);
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(y + 1) >= t.getText().getTextFile().size()) break;
                posns.push_back(posns.back() + t.getText().getTextFile()[y + 1].size() - 1);
                t.getText().backSpace(y + 1, 0);
            }
            posns.pop_back();
            t.getUndo().addRegister(Register{y, x, clone()});
            break;
        }
        default: break;
    }
    t.clearStaticCmd();
    t.displayAllViews();
    switch (getValue()) {
        case TextEditType::YANK_LINE:
        case TextEditType::YANK_MVT:
            throw exceptions::DisplayMessage{rows.size() + " lines yanked"};
            break;
        case TextEditType::DEL_LINE:
        case TextEditType::DEL_MVT:
            throw exceptions::DisplayMessage{rows.size() + " fewer lines"};
            break;
        default: break;
    }
}

void TextEdit::undoAction(TextModel &t, int y, int x) {
    switch (getValue()) {
        case TextEditType::DEL_LINE: {
            t.getText().replaceLine(rows.front(), y);
            for (size_t i = rows.size(); i != 1; --i) 
                t.getText().insert(rows[i - 1], y);
            break;
        }
        case TextEditType::DEL_MVT: undoDelMvt(t, y, x); break;
        case TextEditType::DEL_UNDER: {
            t.getText().insert(rows.front(), y, x);
            break;
        }
        case TextEditType::DEL_BEFORE: {
            t.getText().insert(rows.front(), y, x ? x - 1 : 0);
            break;
        }
        case TextEditType::PUT_BEFORE:
        case TextEditType::PUT_AFTER: undoPut(t, y, x); break;
        case TextEditType::JOIN: {
            for (auto it = posns.rbegin(); it != posns.rend(); ++it) {
                t.getText().newLine(y, *it);
                t.displayAllViews();
            }
            break;
        }
        default: break;
    }
    t.displayAllViews();
}

void TextEdit::delMvt(TextModel &t, int y, int x) {
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
            t.getUndo().addRegister(Register{y, newX, move(clone())});
            break;
        }
        case MvtType::RIGHT:
        case MvtType::WORD_RIGHT: {
            std::string s;
            for (int i = 0; i < newX - x; ++i) s.push_back(t.getText().delChar(y, x));
            rows.emplace_back(s, t.getText().getWidth());
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
        case MvtType::UP: { // go backwards
            for (int i = 0; i < y - newY; ++i) rows.emplace_back(t.getText().delLine(newY));
            rows.emplace_back(t.getText().delLine(newY));
            t.getMove().firstChar(newY, newX);
            t.moveAllCursor(newY, newX);
            t.getUndo().addRegister(Register{newY, x, move(clone())});
            break;
        }
        case MvtType::DOWN: {
            for (int i = 0; i < newY - y; ++i) rows.emplace_back(t.getText().delLine(y));
            rows.emplace_back(t.getText().delLine(y));
            // t.moveAllCursor(newY, 0);
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
        case MvtType::END_CH: {
            std::string s;
            for (int i = 0; i <= newX - x; ++i) {
                s.push_back(t.getText().delChar(y, x));
            }
            rows.emplace_back(s, t.getText().getWidth());
            for (int i = 0; i < newY - y; ++i) {
                rows.emplace_back(t.getText().delLine(y + 1));
            }
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
    }
    t.getClipboard().add(move(clone()));
}

void TextEdit::yankMvt(TextModel &t, int y, int x) {
    int newY = y, newX = x;
    mvt->move(t, newY, newX);
    
    switch (mvt->getValue()) {
        case MvtType::LEFT:
        case MvtType::WORD_LEFT:
        case MvtType::BEG_LINE:
        case MvtType::BEG_CH: {
            std::string s;
            for (int i = 0; i < x - newX; ++i) s.push_back(t.getText().getTextFile()[y][newX + i]);
            rows.emplace_back(s, t.getText().getWidth());
            t.getUndo().addRegister(Register{y, newX, move(clone())});
            break;
        }
        case MvtType::RIGHT:
        case MvtType::WORD_RIGHT: {
            std::string s;
            for (int i = 0; i < newX - x; ++i) s.push_back(t.getText().getTextFile()[y][x + i]);
            rows.emplace_back(s, t.getText().getWidth());
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
        case MvtType::UP: { // go backwards
            for (int i = newY; i <= y; ++i) rows.emplace_back(t.getText().getTextFile()[i]);
            t.getMove().firstChar(newY, newX);
            t.getUndo().addRegister(Register{newY, x, move(clone())});
            break;
        }
        case MvtType::DOWN: {
            for (int i = y; i <= newY; ++i) rows.emplace_back(t.getText().getTextFile()[i]);
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
        case MvtType::END_CH: {
            std::string s;
            for (int i = 0; i <= newX - x; ++i) s.push_back(t.getText().getTextFile()[y][x + i]);
            rows.emplace_back(s, t.getText().getWidth());
            for (int i = y + 1; i <= newY; ++i) rows.emplace_back(t.getText().getTextFile()[i]);
            t.getUndo().addRegister(Register{y, x, move(clone())});
            break;
        }
    }
    t.getClipboard().add(move(clone()));
}

void TextEdit::undoDelMvt(TextModel &t, int y, int x) {
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
            for (size_t i = 1; i < rows.size(); ++i) 
                t.getText().insert(rows[i], y);
            break;
        }
    }
}

void TextEdit::putBefore(TextModel &t, int y, int x) {
    clipboard = t.getClipboard().get()->clone();
    switch (clipboard->getValue()) {
        case TextEditType::DEL_LINE:
        case TextEditType::YANK_LINE: {
            putLineBefore(t, y, x);
            break;
        }
        case TextEditType::DEL_BEFORE:
        case TextEditType::DEL_UNDER: {
            putLinesBefore(t, y, x);
            break;
        }
        case TextEditType::DEL_MVT:
        case TextEditType::YANK_MVT: {
            switch (clipboard->mvt->getValue()) {
                case MvtType::LEFT:
                case MvtType::WORD_LEFT:
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: 
                case MvtType::RIGHT:
                case MvtType::WORD_RIGHT: {
                    putLinesBefore(t, y, x);
                    break;
                }
                case MvtType::UP:
                case MvtType::DOWN: {
                    putLineBefore(t, y, x);
                    break;
                }
                case MvtType::END_CH: {
                    // split string at cursor
                    // cursor pos goes right
                    putLinesMiddle(t, y, x);
                    break;
                }
            }
            break;
        }
        default: break;
    }
}

void TextEdit::putLinesBefore(models::TextModel &t, int y, int x) {
    for (int i = 0; i < getMult(); ++i) 
        t.getText().insert(clipboard->rows.front(), y, x ? x - 1 : 0);
    t.moveCursor(y, x ? x - 1 : 0);
    t.getUndo().addRegister(Register{y, x ? x - 1 : 0, clone()});
}

void TextEdit::putLineBefore(models::TextModel &t, int y, int x) {
    int i = -1;
    for (; i < static_cast<int>(clipboard->rows.size()) * getMult() - 1; i += clipboard->rows.size()) {
        t.getText().insert(clipboard->rows.front(), y + i);
        t.displayAllViews();
        for (size_t j = clipboard->rows.size(); j != 1; --j) {
            t.getText().insert(clipboard->rows[j - 1], y + i);
            t.displayAllViews();
        }
    }
    t.getUndo().addRegister(Register{y, x, clone()});
}

// split string at cursor
// cursor pos goes right
void TextEdit::putLinesMiddle(models::TextModel &t, int y, int x) {
    // substr(0, x-1) to front of first
    // substr(x) to back of last
    // delete row y
    // iterate backwards over rows, insert at y
    // new lines?
    rows.emplace_back(t.getText().getTextFile()[y]);
    if (x != 0) {
        clipboard->rows.front() = t.getText().getTextFile()[y].subRow(0, x - 1) + 
            clipboard->rows.front();
    }
    if (clipboard->rows.size() == 1) {
        clipboard->rows.front() = clipboard->rows.front() + 
            t.getText().getTextFile()[y].subRow(x - 1 < 0 ? 0 : x - 1);
    }
    else {
        clipboard->rows.back() = clipboard->rows.back().subRow(0, 
            clipboard->rows.back().size() - 1) + 
            t.getText().getTextFile()[y].subRow(x - 1 < 0 ? 0 : x - 1);
    }
    t.getText().replaceLine(clipboard->rows.front(), y);
    for (auto r = clipboard->rows.rbegin(); r != clipboard->rows.rend() - 1; ++r) {
        t.getText().insert(*r, y);
    }
    t.getUndo().addRegister(Register{y, x, clone()});
}

void TextEdit::putAfter(TextModel &t, int y, int x) {
    clipboard = t.getClipboard().get()->clone();
    switch (clipboard->getValue()) {
        case TextEditType::DEL_LINE:
        case TextEditType::YANK_LINE: {
            putLinesAfter(t, y, x);
            break;
        }
        case TextEditType::DEL_BEFORE:
        case TextEditType::DEL_UNDER: {
            putLineAfter(t, y, x);
            break;
        }
        case TextEditType::DEL_MVT:
        case TextEditType::YANK_MVT: {
            switch (clipboard->mvt->getValue()) {
                case MvtType::LEFT:
                case MvtType::WORD_LEFT:
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: 
                case MvtType::RIGHT:
                case MvtType::WORD_RIGHT: {
                    putLineAfter(t, y, x);
                    break;
                }
                case MvtType::UP:
                case MvtType::DOWN: {
                    putLinesAfter(t, y, x);
                    break;
                }
                case MvtType::END_CH: {
                    // split string after
                    // cursor position goes left, unless is new line
                    putLinesMiddle(t, y, static_cast<size_t>(x) == 
                        t.getText().getTextFile()[y].size() - 1 ? x : x + 1);
                    break;
                }
            }
        }
        default: break;
    }
}

void TextEdit::putLinesAfter(models::TextModel &t, int y, int x) {
    for (int i = 0; i < static_cast<int>(clipboard->rows.size()) * getMult() - 1; i += clipboard->rows.size()) {
        t.getText().insert(clipboard->rows.front(), y + i);
        for (size_t j = clipboard->rows.size(); j != 1; --j) 
            t.getText().insert(clipboard->rows[j - 1], y + i);
    }
    t.moveAllCursor(y + 1, 0);
    t.getUndo().addRegister(Register{y + 1, x, clone()});
}

void TextEdit::putLineAfter(models::TextModel &t, int y, int x) {
    for (int i = 0; i < getMult(); ++i) 
        t.getText().insert(clipboard->rows.front(), y, x + 1);
    t.moveCursor(y, x + 1);
    t.getUndo().addRegister(Register{y, x + 1, clone()});
}

void TextEdit::undoPut(models::TextModel &t, int y, int x) {
    switch (clipboard->getValue()) {
        case TextEditType::DEL_LINE:
        case TextEditType::YANK_LINE: {
            for (size_t i = 0; i < clipboard->rows.size() * getMult(); ++i)
                t.getText().delLine(y);
            break;
        }
        case TextEditType::DEL_BEFORE:
        case TextEditType::DEL_UNDER: {
            for (size_t i = 0; i < clipboard->rows.front().size(); ++i)
                t.getText().delChar(y, x);
            break;
        }
        case TextEditType::DEL_MVT:
        case TextEditType::YANK_MVT: {
            switch (clipboard->mvt->getValue()) {
                case MvtType::LEFT:
                case MvtType::WORD_LEFT:
                case MvtType::BEG_LINE:
                case MvtType::BEG_CH: 
                case MvtType::RIGHT:
                case MvtType::WORD_RIGHT: {
                    for (size_t i = 0; i < clipboard->rows.front().size(); ++i)
                        t.getText().delChar(y, x);
                    break;
                }
                case MvtType::UP:
                case MvtType::DOWN: {
                    for (size_t i = 0; i < clipboard->rows.size() * getMult(); ++i)
                        t.getText().delLine(y);
                    break;
                }
                case MvtType::END_CH: {
                    t.getText().replaceLine(rows.front(), y);
                    for (size_t i = 0; i < clipboard->rows.size() - 1; ++i)
                        t.getText().delLine(y + 1);
                    break;
                }
            }
            break;
        }
        default: break;
    }
}

unique_ptr<TextEdit> TextEdit::clone() {
    unique_ptr<TextEdit> clip = make_unique<TextEdit>(getValue(), getMult());
    if (mvt) clip->mvt = make_unique<Movement>(*mvt.get());
    clip->posns = posns;
    clip->rows = rows;
    if (clipboard) clip->clipboard = clipboard->clone();
    return clip;
}
}
