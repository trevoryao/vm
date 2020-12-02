#include "action.h"

#include "bad-entry.h"

namespace actions {
Action::Action(ActionType type, int n) : type{type}, mult{n} { }

ActionType Action::getType() { return type; }

int Action::getMult() { return mult; }
}
