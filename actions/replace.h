#ifndef __REPLACE_H__
#define __REPLACE_H__

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class ReplaceType { RPL_UNDER, RPL };

class Replace final : public IAction<ReplaceType> {
public:
    Replace(ReplaceType value, int n = 1);
    
    void execAction(models::TextModel &t) { }
};
}

#endif
