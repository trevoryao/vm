#ifndef __ACTION_BASE_H__
#define __ACTION_BASE_H__

namespace actions {
class Action {
private:
    int mult;
public:
    Action(int n);
    
    virtual ~Action() = default;
    
    int getMult();
};
}

#endif
