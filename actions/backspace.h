#ifndef __BACKSPACE_H__
#define __BACKSPACE_H__

#include <list>
#include <string>

#include "buffer.h"

namespace models {
class TextModel;
}

// group of text
namespace actions {
enum class KeyType;

class Backspace final : public Buffer {
private:
    int y, x2;
    std::list<char> buffer;
public:
    Backspace(char c, int y);
    Backspace(const std::string &s, int y);
    
    void addEvent(char c, int x) override;
    
    bool canAdd(KeyType value) override;
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int, int) override;
};
}

#endif
