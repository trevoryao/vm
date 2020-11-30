#ifndef __INSERT_H__
#define __INSERT_H__

#include <memory>

#include "i-action.h"
#include "movement.h"

namespace actions {
enum class InsType {
    BEFORE,
    AFTER,
    NEW_LINE_ABOVE,
    NEW_LINE_BELOW,
    BEG_LINE,
    END_LINE,
    CH_MVT,
    CH_LINE,
    CH
};

using std::unique_ptr;

class Insert final: public IAction<InsType> {
private:
    unique_ptr<Movement> mvt;
public:
    Insert(InsType value, int n = 1, unique_ptr<Movement> mvt = unique_ptr<Movement>{});
    
    Movement *getMvt();
};
}

#endif
