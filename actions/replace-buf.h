#ifndef __REPLACE_BUF_H__
#define __REPLACE_BUF_H__

#include <string>

#include "buffer.h"

namespace models {
class TextModel;
}

namespace actions {
enum class KeyType;

class ReplaceBuf final : public Buffer {
private:
    int y, x1, x2;
    std::string original;
    std::string replacement;
public:
    ReplaceBuf(char r, char c, int y, int x);
    
    void addEvent(char r, char c, int x) override;
    
    bool canAdd(KeyType value) override;
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int, int) override;
};
}

#endif
