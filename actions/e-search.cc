#include "e-search.h"

#include <string>

namespace actions {
ESearch::ESearch(ESearchType value, int n, const std::string &search) :
    IAction{value, n}, search{search} { }

const std::string &ESearch::getSearch() { return search; }
}
