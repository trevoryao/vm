#ifndef __ADD_H__
#define __ADD_H__

#include <string>
#include <utility>

#include "buffer.h"

namespace models {
class TextModel;
}

// group of text
namespace actions {
enum class KeyType;

class Add final : public Buffer {
private:
    int y, x1, x2;
    std::string buffer;
public:
    Add(char c, int y, int x);
    Add(const std::string &s, int y, int x);
    
    void addEvent(char c, char, int x) override;
    
    bool canAdd(KeyType value) override;
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int, int) override;
};
}

#endif
