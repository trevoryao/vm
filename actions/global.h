#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "i-action.h"

#include <string>

namespace actions {
enum class GlobalType { NONE, RESIZE, DISPLAY_FILE };

class Global final : public IAction<GlobalType> {
public:
    Global(GlobalType value);
};
}

#endif
