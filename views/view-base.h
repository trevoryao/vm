#ifndef __VIEW_BASE_H__
#define __VIEW_BASE_H__

#include <string>

#include "../ui/window.h"

namespace views {
class ViewBase {
protected:
    ui::Window window;
public:
    ViewBase(int height, int width, int y, int x);
    
    virtual ~ViewBase() = default;
    // TODO: update for each type of view, char, string, type
    virtual void update(char c, int y, int x) = 0;
    virtual void update(const std::string &s, int y, int x) = 0;
    
    virtual void updateExec(const std::string &s);
    virtual void clearExec();
    
    virtual void updateStatic(const std::string &s);
    virtual void clearStatic();
    
    virtual void writeMessage(const std::string &s);
    virtual void writeWarning(const std::string &s);
    
    virtual void displayView() = 0;
    
    virtual void resizeView() = 0;
    
    virtual int getMaxHeight() = 0;
    virtual int getMaxWidth() = 0;

    virtual void moveCursor(int y, int x) = 0;
};
}

#endif
