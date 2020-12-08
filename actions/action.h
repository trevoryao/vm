#ifndef __ACTION_H__
#define __ACTION_H__

namespace models {
class TextModel;
}

namespace actions {
class Action {
private:
    int mult;
public:
    Action(int n);
    
    virtual ~Action() = default;
    
    int getMult();
    
    virtual void execAction(models::TextModel &t) = 0;
    virtual void undoAction(models::TextModel &t, int y, int x);
};
}

#endif
