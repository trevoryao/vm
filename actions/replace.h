#ifndef __REPLACE_H__
#define __REPLACE_H__

#include <vector>

#include "i-action.h"
#include "../models/row.h"

namespace models {
class TextModel;
}

namespace actions {
enum class ReplaceType { RPL_UNDER, RPL };

class Replace final : public IAction<ReplaceType> {
private:
    char ins;
    std::vector<models::Row> buffer;
public:
    Replace(ReplaceType value, char c, int n = 1);
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int y, int x) override;
};
}

#endif
