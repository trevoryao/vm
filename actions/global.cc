#include "global.h"

#include "../models/text-model.h"

#include <string>

namespace actions {
Global::Global(GlobalType value) : IAction{value} { }

void Global::execAction(models::TextModel &t) {
    switch(getValue()) {
        case GlobalType::NONE: break;
        case GlobalType::RESIZE: t.resizeViews(); break;
        case GlobalType::DISPLAY_FILE: t.displayName(); break; // TODO
    }
}
}
