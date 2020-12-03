#include "text-edit.h"

#include <memory>
#include <utility>

#include "movement.h"

namespace actions {
TextEdit::TextEdit(TextEditType value, int n, std::unique_ptr<Movement> mvt) :
    IAction{value, n}, mvt{std::move(mvt)} { }

Movement *TextEdit::getMvt() { return mvt.get(); }
}
