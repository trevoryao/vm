#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class MvtType {
    // NONE, // null
    LEFT, // h
    RIGHT, // l
    UP, // k
    DOWN, // j
    WORD_LEFT, // b
    WORD_RIGHT, // w
    BEG_LINE, // 0
    BEG_CH, // ^
    END_CH, // $
};

class Movement final : public IAction<MvtType> {
public:
    Movement(MvtType value, int n = 1);
    
    void execAction(models::TextModel &t) override;
};
}

#endif
