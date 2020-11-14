#include "model-base.h"

#include <memory>
#include <vector>
#include <utility>

#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using namespace std;

namespace vm {
void ModelBase::addView(unique_ptr<ViewBase> v) {
    views.emplace_back(move(v));
}

void ModelBase::addController(unique_ptr<ControllerBase> c) {
    controllers.emplace_back(move(c));
}
}
