#include "text-edit.h"

#include <memory>
#include <string>
#include <utility>

#include "../models/text-model.h"

/*
textEditMap{
        {"d", TextEditType::DEL_MVT},
        {'p', TextEditType::PUT_AFTER},
        {'P', TextEditType::PUT_BEFORE},
        {"y", TextEditType::YANK_MVT},
    },
*/

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
            rows.emplace_back(t.getText().changeLine(y));
            for (int i = 1; i < getMult(); ++i) {
                if (static_cast<size_t>(y + 1) >= t.getText().getTextFile().size()) break;
                rows.push_back(t.getText().delLine(y + 1));
            }
            t.getClipboard().add(move(clone()));
            t.getUndo().addRegister(Register{y, x, make_unique<TextEdit>(move(*this))});
            break;
        }
        case TextEditType::DEL_UNDER: {
            std::string s;
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(x) >= t.getText().getTextFile()[y].size() - 1) break;
                s.push_back(t.getText().delChar(y, x));
            }
            rows.emplace_back(s, t.getText().getWidth());
            t.getClipboard().add(move(clone()));
            t.getUndo().addRegister(Register{y, x, make_unique<TextEdit>(move(*this))});
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
            t.getUndo().addRegister(Register{y, x - getMult() < 0 ? 0 : x - getMult(), 
                make_unique<TextEdit>(move(*this))});
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
        case TextEditType::JOIN: {
            posns.push_back(t.getText().getTextFile()[y].size() - 1);
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(y + 1) >= t.getText().getTextFile().size()) break;
                posns.push_back(posns.back() + t.getText().getTextFile()[y + 1].size() - 1);
                t.getText().backSpace(y + 1, 0);
            }
            posns.pop_back();
            t.getUndo().addRegister(Register{y, x, make_unique<TextEdit>(move(*this))});
            break;
        }
        default: break;
    }
    t.clearStaticCmd();
    t.displayAllViews();
}

void TextEdit::undoAction(TextModel &t, int y, int x) {
    switch (getValue()) {
        case TextEditType::DEL_LINE: {
            t.getText().replaceLine(rows.front(), y);
            for (size_t i = rows.size(); i != 1; --i) 
                t.getText().insert(rows[i - 1], y);
            break;
        }
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

unique_ptr<TextEdit> TextEdit::clone() {
    unique_ptr<TextEdit> clip = mvt ? 
        make_unique<TextEdit>(getValue(), getMult(), 
            make_unique<Movement>(*mvt.get()))
        : make_unique<TextEdit>(getValue(), getMult());
    clip->posns = posns;
    clip->rows = rows;
    if (clipboard) clip->clipboard = clipboard->clone();
    return clip;
}

void TextEdit::putBefore(TextModel &t, int y, int x) {
    clipboard = t.getClipboard().get()->clone();
    switch (clipboard->getValue()) {
        case TextEditType::DEL_LINE:
        case TextEditType::YANK_LINE: {
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
            break;
        }
        case TextEditType::DEL_BEFORE:
        case TextEditType::DEL_UNDER: {
            for (int i = 0; i < getMult(); ++i) 
                t.getText().insert(clipboard->rows.front(), y, x ? x - 1 : 0);
            t.moveCursor(y, x ? x - 1 : 0);
            t.getUndo().addRegister(Register{y, x ? x - 1 : 0, clone()});
            break;
        }
        default: break;
    }
}

void TextEdit::putAfter(TextModel &t, int y, int x) {
    clipboard = t.getClipboard().get()->clone();
    switch (clipboard->getValue()) {
        case TextEditType::DEL_LINE:
        case TextEditType::YANK_LINE: {
            for (int i = 0; i < static_cast<int>(clipboard->rows.size()) * getMult() - 1; i += clipboard->rows.size()) {
                t.getText().insert(clipboard->rows.front(), y + i);
                for (size_t j = clipboard->rows.size(); j != 1; --j) 
                    t.getText().insert(clipboard->rows[j - 1], y + i);
            }
            t.moveAllCursor(y + 1, 0);
            t.getUndo().addRegister(Register{y + 1, x, clone()});
            break;
        }
        case TextEditType::DEL_BEFORE:
        case TextEditType::DEL_UNDER: {
            for (int i = 0; i < getMult(); ++i) 
                t.getText().insert(clipboard->rows.front(), y, x + 1);
            t.moveCursor(y, x + 1);
            t.getUndo().addRegister(Register{y, x + 1, clone()});
            break;
        }
        default: break;
    }
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
        default: break;
    }
}
}
