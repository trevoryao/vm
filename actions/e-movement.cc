#include "e-movement.h"

namespace actions {
EMovement::EMovement(EMvtType value, int mult, int lineNum) : 
    IAction{value, mult}, lineNum{lineNum} { }

int EMovement::getLineNum() { return lineNum; }
}
