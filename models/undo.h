#ifndef __UNDO_H__
#define __UNDO_H__

#include <memory>
#include <vector>

#include "register.h"
#include "row.h"
#include "../actions/buffer.h"

namespace models {
class TextModel;

class Undo final {
private:
    std::vector<Register> registers;
    std::unique_ptr<actions::Buffer> textBuffer;
public:
    Undo() = default;
    
    void addRegister(Register r);
    
    void undo(TextModel &t);
    
    void execLastRegister(TextModel &t);
    
    void setBuffer(std::unique_ptr<actions::Buffer> textBuffer);
    actions::Buffer *getBuffer();
    void addBuffer();
    
    bool hasBuffer();
};
}

#endif
