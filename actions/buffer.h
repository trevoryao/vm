#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "action.h"

namespace actions {
enum class KeyType;

class Buffer : public Action {
public:
    Buffer();
    
    virtual bool canAdd(KeyType value) = 0;
    virtual void addEvent(char c, char r, int x) = 0;
};
}

#endif
