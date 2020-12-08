#ifndef __INSERT_H__
#define __INSERT_H__

#include <memory>
#include <vector>

#include "i-action.h"
#include "movement.h"
#include "../models/row.h"

using std::unique_ptr;

namespace models {
class TextModel;
}

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

class Insert final: public IAction<InsType> {
private:
    unique_ptr<Movement> mvt;
    std::vector<models::Row> rows;
public:
    Insert(InsType value, int n = 1, unique_ptr<Movement> mvt = unique_ptr<Movement>{});
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int y, int x) override;
};
}

#endif
