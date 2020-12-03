#ifndef __I_ACTION_H__
#define __I_ACTION_H__

#include "action.h"

namespace actions {
template<typename E> class IAction : public Action {
private:
    E value;
public:
    IAction(E value, int n = 1);
    
    virtual ~IAction();

    E getValue();
    // void setValue(E value);
};

template<typename E> IAction<E>::IAction(E value, int n) : Action{n}, value{value} { }

template<typename E> IAction<E>::~IAction() { }

template<typename E> E IAction<E>::getValue() { return value; }
// template<typename E> void IAction<E>::setValue(E value) { this->value = value; }
}

#endif
