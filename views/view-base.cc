#include "view-base.h"

#include <utility>

#include "../ui/window.h"

namespace views{
ViewBase::ViewBase(int height, int width, int y, int x): window{height, width, y, x} { }

void ViewBase::updateExec(const std::string &) { }
void ViewBase::clearExec() { }
void ViewBase::updateStatic(const std::string &) { }
void ViewBase::clearStatic() { }
void ViewBase::writeMessage(const std::string &) { }
}
