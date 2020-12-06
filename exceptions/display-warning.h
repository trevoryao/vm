#ifndef __DISPLAY_WARNING_H__
#define __DISPLAY_WARNING_H__

#include <string>

namespace exceptions {
class DisplayWarning {
private:
    std::string warning;
public:
    explicit DisplayWarning(const std::string &warning);
    explicit DisplayWarning(std::string &&warning);
    
    const std::string &getWarning();
};
}

#endif
