#include "replace.h"

#include "action.h"

namespace actions {
Replace::Replace(ReplaceType value, int n) : IAction{ActionType::REPLACE, value, n} { }
}