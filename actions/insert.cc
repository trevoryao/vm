#include "insert.h"

#include <utility>

namespace actions {
Insert::Insert(InsType value, int n, std::unique_ptr<Movement> mvt) : 
    IAction{value, n}, mvt{std::move(mvt)} { }

Movement *Insert::getMvt() { return mvt.get(); }
}
