#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <string>
#include <memory>

#include "../actions/e-search.h"
#include "../actions/action.h"
#include "../actions/incomplete.h"
#include "../actions/search.h"

using actions::Action;
using actions::Incomplete;
using std::unique_ptr;

namespace models {
class TextModel;
}

namespace controllers {
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    
    unique_ptr<Action> getAction();
    unique_ptr<Action> getAction(Incomplete *a);
protected:
    virtual unique_ptr<Action> action() = 0;
    virtual unique_ptr<Action> action(Incomplete *a) = 0;
};
}

#endif
