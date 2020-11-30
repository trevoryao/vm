#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <string>
#include <memory>

#include "../actions/e-search.h"
#include "../actions/action.h"
#include "../actions/incomplete.h"
#include "../actions/search.h"

using actions::Action;
using actions::ESearch;
using actions::Incomplete;
using actions::Search;
using std::unique_ptr;

namespace controllers {
class ControllerBase {
protected:
    virtual unique_ptr<Action> action() = 0;
    virtual unique_ptr<Action> action(Incomplete *a) = 0;
    virtual unique_ptr<Action> action(ESearch *a) = 0;
    virtual unique_ptr<Action> action(Search *a) = 0;
public:
    virtual ~ControllerBase() = default;
    
    unique_ptr<Action> getAction();
    unique_ptr<Action> getAction(Incomplete *a);
    unique_ptr<Action> getAction(ESearch *a);
    unique_ptr<Action> getAction(Search *a);
};
}

#endif
