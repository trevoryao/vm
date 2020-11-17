#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "action.h"
#include "action-enums.h"

namespace actions {
class Movement final : public Action {
private:
    MovementType mvt;
public:
    Movement(MovementType mvt);
    
    MovementType getMvt();
};
}

#endif
