#ifndef __INCOMPLETE_H__
#define __INCOMPLETE_H__

#include <string>

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class IncType { UNKNOWN, EXEC, STATIC };

// usually either full of integers, or so exec command
class Incomplete final : public IAction<IncType> {
private:
    std::string fragment;
public:
    Incomplete(IncType value, const std::string &s = "", int n = 1);
    Incomplete(IncType value, char c, int n = 1);
    
    const std::string &getFragment();
    
    void addFragment(char c);
    
    void execAction(models::TextModel &t) override;
};
}

#endif
