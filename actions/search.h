#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "i-action.h"

namespace actions {
enum class SearchType {
    NEXT_CHAR,
    PREV_CHAR,
    REPEAT_CHAR,
    FWD_SEARCH,
    PREV_SEARCH,
};

class Search final : public IAction<SearchType> {
private:
    char search;
public:
    Search(SearchType value, int n = 1, char search = -1);
    
    char getSearch();
};
}

#endif
