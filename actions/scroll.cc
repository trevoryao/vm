#include "scroll.h"

#include "action.h"

namespace actions {
Scroll::Scroll(ScrollType value) : IAction{ActionType::SCROLL, value, 
    (value == ScrollType::PG_FWD || value == ScrollType::PG_BACK) ? 1 : -1} { }

Scroll::Scroll(ScrollType value, int n) : IAction{ActionType::SCROLL, value, n} { }
}
