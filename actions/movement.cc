#include "movement.h"

#include "action.h"

namespace actions {
Movement::Movement(MvtType value, int n) : IAction{ActionType::MVT, value, n} { }
}
