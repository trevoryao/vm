#include "incomplete.h"

#include "action.h"

#include <string>

namespace actions {
Incomplete::Incomplete(IncType value, const std::string &s, int n) : 
    IAction{ActionType::INCOMPLETE, value, n}, fragment{s} { }

Incomplete::Incomplete(IncType value, char c, int n) : 
    IAction{ActionType::INCOMPLETE, value, n}, fragment{1, c} { }

const std::string &Incomplete::getFragment() { return fragment; }

void Incomplete::addFragment(char c) { fragment.push_back(c); }
}
