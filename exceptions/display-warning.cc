#include "display-warning.h"

#include <string>
#include <utility>

namespace exceptions {
DisplayWarning::DisplayWarning(const std::string &warning) : warning{warning} { }

DisplayWarning::DisplayWarning(std::string &&warning) : warning{std::move(warning)} { }

const std::string &DisplayWarning::getWarning() { return warning; }
}
