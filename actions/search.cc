#include "search.h"

#include <utility>

#include "../models/text-model.h"

using namespace models;
using namespace std;

namespace actions {
Search::Search(SearchType value, int n, char search) : 
    IAction{value, n}, search{search} { }

void Search::execAction(TextModel &t) {
    int y, x;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case SearchType::REPEAT_CHAR: {
            auto *last = t.getSearch().getSearch();
            if (!last) break;
            if (last->getValue() == SearchType::NEXT_CHAR) {
                t.getSearch().searchFwd(y, x, last->search, getMult());
            } else {
                t.getSearch().searchBwd(y, x, last->search, getMult());
            }
            break;
        }
        case SearchType::NEXT_CHAR: 
            t.getSearch().searchFwd(y, x, search, getMult());
        case SearchType::PREV_CHAR:
            t.getSearch().searchBwd(y, x, search, getMult());
        default:
            t.getSearch().setSearch(make_unique<Search>(*this));
            break;
    }
    
    t.moveAllCursor(y, x);
    t.clearStaticCmd();
    t.displayAllViews();
}
}
