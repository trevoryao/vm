#include "text-edit.h"

#include <memory>
#include <utility>

#include "action.h"
#include "movement.h"

namespace actions {
TextEdit::TextEdit(TextEditType value, int n, std::unique_ptr<Movement> mvt) :
    IAction{ActionType::TEXT_EDIT, value, n}, mvt{std::move(mvt)} { }

Movement *TextEdit::getMvt() { return mvt.get(); }
}
