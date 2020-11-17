#ifndef __VIEW_BASE_H__
#define __VIEW_BASE_H__

#include <string>

namespace views {
class ViewBase {
public:
    virtual ~ViewBase() = default;
    // TODO: update for each type of view, char, string, type
    virtual void update(char c) = 0;
    virtual void update(char c, int y, int x) = 0;
    virtual void update(const std::string &s) = 0;
    virtual void update(const std::string &s, int y, int x) = 0;
    
    virtual void displayView() = 0;
    
    virtual void resizeView() = 0;
};
}

#endif
