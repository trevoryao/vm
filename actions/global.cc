#include "global.h"

#include "action.h"

#include <string>

namespace actions {
Global::Global(GlobalType value) : IAction{ActionType::GLOBAL, value} { }
}
