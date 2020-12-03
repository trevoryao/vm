#include "scroll.h"

namespace actions {
Scroll::Scroll(ScrollType value) : IAction{value,
    (value == ScrollType::PG_FWD || value == ScrollType::PG_BACK) ? 1 : -1} { }

Scroll::Scroll(ScrollType value, int n) : IAction{value, n} { }
}
