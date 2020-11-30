#include "search.h"

namespace actions {
Search::Search(SearchType value, int n, char search) : IAction{value, n}, search{search} { }

char Search::getSearch() { return search; }
}
