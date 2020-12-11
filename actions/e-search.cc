#include "e-search.h"

#include <string>
#include <utility>

#include "../exceptions/display-message.h"
#include "../exceptions/display-warning.h"
#include "../models/text-model.h"

using namespace exceptions;
using namespace std;
using namespace models;

namespace actions {
ESearch::ESearch(ESearchType value, int n, const std::string &search) :
    IAction{value, n}, search{search} { }

const std::string &ESearch::getSearch() { return search; }

void ESearch::execAction(models::TextModel &t) {
    int y, x, cycles;
    bool fwd = false;
    t.getCursor(y, x);
    
    switch (getValue()) {
        case ESearchType::REPEAT: {
            auto *last = t.getSearch().getESearch();
            if (!last) break;
            if (last->getValue() == ESearchType::NEW_FWD_SEARCH) {
                if (!t.getSearch().searchFwd(y, x, last->search, getMult(), cycles)) {
                    throw DisplayWarning{"Pattern not found: " + last->search};
                }
                fwd = true;
            } else {
                if (!t.getSearch().searchBwd(y, x, last->search, getMult(), cycles)) {
                    throw DisplayWarning{"Pattern not found: " + last->search};
                }
            }
            break;
        }
        case ESearchType::REPEAT_OPP: {
            auto *last = t.getSearch().getESearch();
            if (!last) break;
            if (last->getValue() == ESearchType::NEW_FWD_SEARCH) {
                if (!t.getSearch().searchBwd(y, x, last->search, getMult(), cycles)) {
                    throw DisplayWarning{"Pattern not found: " + last->search};
                }
            } else {
                if (!t.getSearch().searchFwd(y, x, last->search, getMult(), cycles)) {
                    throw DisplayWarning{"Pattern not found: " + last->search};
                }
                fwd = true;
            }
            break;
        }
        case ESearchType::NEW_FWD_SEARCH:
            t.getSearch().setESearch(make_unique<ESearch>(*this));
            if (!t.getSearch().searchFwd(y, x, search, getMult(), cycles))
                throw DisplayWarning{"Pattern not found: " + search};
            fwd = true;
            break;
        case ESearchType::NEW_PREV_SEARCH:
            t.getSearch().setESearch(make_unique<ESearch>(*this));
            if (!t.getSearch().searchBwd(y, x, search, getMult(), cycles))
                throw DisplayWarning{"Pattern not found: " + search};
            break;
    }
    
    t.moveAllCursor(y, x);
    t.displayAllViews();
    t.clearStaticCmd();
    // check for a full cycle, if found == 0, return false Pattern not found
    // otherwise search hit BOTTOM, continuing at TOP
    if (cycles != 0) {
        if (fwd) throw DisplayMessage{"search hit BOTTOM, continuing at TOP"};
        else throw DisplayMessage{"search hit TOP, continuing at BOTTOM"};
    }
}
}
