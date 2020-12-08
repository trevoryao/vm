#include "register.h"

#include <memory>
#include <utility>

#include "text-model.h"
#include "../actions/action.h"

using namespace actions;
using namespace std;

namespace models {
Register::Register(int y, int x, unique_ptr<Action> action) : y{y}, x{x}, 
    action{move(action)} { }

/*
int Register::getY() { return y; }

int Register::getX() { return x; }

Action *Register::getAction() { return action.get(); }*/

void Register::execAction(TextModel &t) { action->execAction(t); }

void Register::undoAction(TextModel &t) { action->undoAction(t, y, x); }
}
