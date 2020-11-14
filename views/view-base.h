#ifndef __VIEW_BASE_H__
#define __VIEW_BASE_H__

namespace vm {
class ViewBase {
public:
    virtual ~ViewBase() = default;
    
    virtual void update() = 0;
    virtual void displayView() = 0;
};
}

#endif
