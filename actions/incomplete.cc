#include "incomplete.h"

#include "action.h"
#include "../models/text-model.h"

#include <string>

namespace actions {
Incomplete::Incomplete(IncType value, char c) : 
    IAction{value, 1}, printFrag{false}, fragment(1, c) { }

Incomplete::Incomplete(IncType value, char c, int n) : 
    IAction{value, n}, printFrag{true}, fragment(1, c) { }

std::string Incomplete::getStaticFragment() {
    return (printFrag ? std::to_string(getMult()) : "") + fragment;
}

const std::string &Incomplete::getFragment() { return fragment; }

void Incomplete::addFragment(char c) { fragment.push_back(c); }

void Incomplete::removeFragment() { fragment.pop_back(); }

void Incomplete::execAction(models::TextModel &t) {
    if (getValue() == IncType::EXEC) {
        t.setExecCmd(this);
    } else {
        t.setStaticCmd(this);
    }
}
}
