#include "model-base.h"

#include <memory>
#include <vector>
#include <utility>

#include "mode-type.h"
#include "../actions/i-action.h"
#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using namespace actions;
using namespace controllers;
using namespace std;
using namespace views;

namespace models {
void ModelBase::addView(unique_ptr<ViewBase> v) {
    views.emplace_back(move(v));
}

void ModelBase::addInputController(unique_ptr<ControllerBase> c) {
    inputController = move(c);
}

void ModelBase::addKeyController(unique_ptr<ControllerBase> c) {
    keyController = move(c);
}

unique_ptr<Action> ModelBase::getAction(ModeType m) {
    return m == ModeType::CMD ? inputController->getAction() : keyController->getAction();
}

unique_ptr<Action> ModelBase::getAction(Incomplete *a) {
    return inputController->getAction(a);
}

unique_ptr<Action> ModelBase::getAction(ESearch *a) {
    return inputController->getAction(a);
}

unique_ptr<Action> ModelBase::getAction(Search *a) {
    return inputController->getAction(a);
}

void ModelBase::displayViews() {
    for (auto &v : views) v->displayView();
}

void ModelBase::displayInfo() {
    for (auto &v : views) v->displayInfo();
}

void ModelBase::resizeViews() {
    for (auto &v : views) v->resizeView();
}

int ModelBase::getHeight() {
    int height = 0;
    for (auto &v : views) height += v->getMaxHeight();
    return height;
}

void ModelBase::moveCursor(int y, int x) {
    for (auto &v : views) v->moveCursor(y, x);
}

void ModelBase::updateExecView(const std::string &s) {
    for (auto &v : views) v->updateExec(s);
}

void ModelBase::updateWarning(const std::string &s) {
    for (auto &v : views) v->writeWarning(s);
}

void ModelBase::clearExecView() { 
    for (auto &v : views) v->clearExec();
}

void ModelBase::updateStaticView(const std::string &s) {
    for (auto &v : views) v->updateStatic(s);
}

void ModelBase::clearStaticView() { 
    for (auto &v : views) v->clearStatic();
}

void ModelBase::updateView(std::string &s, int y, int x) {
    for (auto &v : views) v->update(s, y, x);
}

void ModelBase::writeMessage(const std::string &s) {
    for (auto &v : views) v->writeMessage(s);
}

void ModelBase::writeMode(const std::string &s) {
    for (auto &v : views) v->writeMode(s);
}
}
