#ifndef __DELETE_H__
#define __DELETE_H__

#include <vector>
#include <string>

#include "buffer.h"

namespace models {
class TextModel;
}

// group of text
namespace actions {
enum class KeyType;

class Delete final : public Buffer {
private:
    int y, x1;
    std::vector<char> buffer;
public:
    Delete(char c, int y, int x);
    Delete(const std::string &s, int y, int x);
    
    void addEvent(char c, int x) override;
    
    bool canAdd(KeyType value) override;
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int, int) override;
};
}

#endif
