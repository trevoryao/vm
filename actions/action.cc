#include "action.h"

namespace actions {
Action::Action(ActionType type): type{type} { }

ActionType Action::getType() { return type; }
}
