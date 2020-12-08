#include "text-edit.h"

#include <memory>
#include <utility>

#include "movement.h"
#include "../models/text-model.h"

namespace actions {
TextEdit::TextEdit(TextEditType value, int n, std::unique_ptr<Movement> mvt) :
    IAction{value, n}, mvt{std::move(mvt)} { }

void TextEdit::execAction(models::TextModel &t) {
    switch (getValue()) {
        case TextEditType::UNDO: t.getUndo().undo(t); break;
        default: break;
    }
    t.displayAllViews();
}
}
