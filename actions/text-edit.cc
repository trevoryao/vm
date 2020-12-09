#include "text-edit.h"

#include <memory>
#include <utility>

#include "movement.h"
#include "../models/text-model.h"
#include "../models/row.h"

/*
textEditMap{
        {"dd", TextEditType::DEL_LINE},
        {"d", TextEditType::DEL_MVT},
        {'x', TextEditType::DEL_UNDER},
        {'X', TextEditType::DEL_BEFORE},
        {'p', TextEditType::PUT_AFTER},
        {'P', TextEditType::PUT_BEFORE},
        {"yy", TextEditType::YANK_LINE},
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
        case TextEditType::UNDO: t.getUndo().undo(t); break;
        case TextEditType::REPEAT: t.getUndo().execLastRegister(t); break;
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
    t.displayAllViews();
}

void TextEdit::undoAction(models::TextModel &t, int y, int x) {
    switch (getValue()) {
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
}
