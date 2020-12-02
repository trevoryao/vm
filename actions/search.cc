#include "search.h"

#include "action.h"

namespace actions {
Search::Search(SearchType value, int n, char search) : 
    IAction{ActionType::SEARCH, value, n}, search{search} { }

char Search::getSearch() { return search; }
}
