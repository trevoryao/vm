#include "movement.h"

namespace actions {
Movement::Movement(MovementType mvt): Action{MVT}, mvt{mvt} { }

MovementType Movement::getMvt() { return mvt; }
}
