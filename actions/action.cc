#include "action.h"

#include "../models/text-model.h"

namespace actions {
Action::Action(int n) : mult{n} { }

int Action::getMult() { return mult; }

void Action::undoAction(models::TextModel &, int, int) { }
}
