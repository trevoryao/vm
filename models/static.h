#ifndef __STATIC_H__
#define __STATIC_H__

#include <memory>

#include "../actions/incomplete.h"

namespace models {
class Static {
private:
    std::unique_ptr<actions::Incomplete> cmd;
public:
    Static();
    Static(std::unique_ptr<actions::Incomplete> cmd);
    
    
};
}

#endif
