#include "e-movement.h"

#include "action.h"

namespace actions {
EMovement::EMovement(EMvtType value, int mult, int lineNum) : 
    IAction{ActionType::E_MVT, value, mult}, lineNum{lineNum} { }

int EMovement::getLineNum() { return lineNum; }
}
