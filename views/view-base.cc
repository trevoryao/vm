#include "view-base.h"

#include <utility>

#include "../ui/window.h"

namespace views{
ViewBase::ViewBase(int height, int width, int y, int x): window{height, width, y, x} { }
}
