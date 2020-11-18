#include "model-base.h"

#include <memory>
#include <vector>
#include <utility>

#include "../actions/action.h"
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

unique_ptr<actions::Action> ModelBase::getAction(int i) {
    return controllers[i]->action();
}

void ModelBase::displayViews() {
    for (auto &v : views) v->displayView();
}

void ModelBase::resizeViews() {
    for (auto &v : views) v->resizeView();
}

void ModelBase::moveCursor(int y, int x) {
    for (auto &v : views) v->moveCursor(y, x);
}
}
