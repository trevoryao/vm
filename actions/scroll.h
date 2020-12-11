#ifndef __SCROLL_H__
#define __SCROLL_H__

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class ScrollType {
    PG_FWD,
    PG_BACK,
    LINE_FWD,
    LINE_BACK
};

class Scroll final : public IAction<ScrollType> {
public:
    Scroll(ScrollType value);
    Scroll(ScrollType value, int n);
    
    void execAction(models::TextModel &t) override;
};
}

#endif
