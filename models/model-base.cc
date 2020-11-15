#include "model-base.h"

#include <memory>
#include <vector>
#include <utility>

#include "../controllers/action.h"
#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using namespace std;

using namespace controllers;
using namespace views;

namespace models {
void ModelBase::addView(unique_ptr<ViewBase> v) {
    views.emplace_back(move(v));
}

void ModelBase::addController(unique_ptr<ControllerBase> c) {
    controllers.emplace_back(move(c));
}

void ModelBase::displayViews() {
    for (auto &v : views) v->displayView();
}

controllers::Action ModelBase::getAction() {
    return v[0]->action();
}
}
