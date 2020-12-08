#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <memory>

#include "../actions/action.h"

namespace models {
class TextModel;

class Register final {
private:
    int y, x;
    std::unique_ptr<actions::Action> action;
public:
    Register(int y, int x, std::unique_ptr<actions::Action> action);
    
    /*
    int getY();
    int getX();
    actions::Action *getAction();*/
    
    void execAction(TextModel &t);
    void undoAction(TextModel &t);
};
}

#endif
