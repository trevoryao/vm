#include "controller-base.h"

#include <string>
#include <memory>
#include <utility>

#include "../actions/e-search.h"
#include "../actions/action.h"
#include "../actions/incomplete.h"
#include "../actions/search.h"
#include "../models/text-model.h"

using namespace actions;
using namespace std;

namespace controllers {
unique_ptr<Action> ControllerBase::getAction() { return action(); }
unique_ptr<Action> ControllerBase::getAction(Incomplete *a) { return action(a); }
}
