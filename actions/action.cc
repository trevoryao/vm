#include "action.h"

#include "bad-entry.h"

namespace actions {
Action::Action(int n) : mult{n} { }

void Action::getAction(int c) { throw BadEntry{c}; }
}
