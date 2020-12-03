#include "incomplete.h"

#include "action.h"
#include "../models/text-model.h"

#include <string>

namespace actions {
Incomplete::Incomplete(IncType value, const std::string &s, int n) : 
    IAction{value, n}, fragment{s} { }

Incomplete::Incomplete(IncType value, char c, int n) : 
    IAction{value, n}, fragment{1, c} { }

const std::string &Incomplete::getFragment() { return fragment; }

void Incomplete::addFragment(char c) { fragment.push_back(c); }

void Incomplete::execAction(models::TextModel &t) {
    if (getValue() == IncType::EXEC) {
        t.setExecCmd(this);
    } else {
        t.setStaticCmd(this);
    }
}
}
