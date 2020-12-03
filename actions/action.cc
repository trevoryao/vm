#include "action.h"

#include "bad-entry.h"

namespace actions {
Action::Action(int n) : mult{n} { }

int Action::getMult() { return mult; }
}
