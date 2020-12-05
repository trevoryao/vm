#ifndef __MOVE_H__
#define __MOVE_H__

#include "text.h"

namespace models {
enum class ModeType;

class Move final {
private:
    Text &text;
public:
    explicit Move(Text &text);
    
    void left(int &y, int &x, int n);
    void right(int &y, int &x, int n, ModeType mode);
    void up(int &y, int &x, int n, ModeType mode);
    void down(int &y, int &x, int n, ModeType mode);
    
    void wordFwd(int &y, int &x, int n);
    void wordBwd(int &y, int &x, int n);
    
    void firstChar(int &y, int &x, int n = 1);
    void lastChar(int &y, int &x, int lines, int n = 1);
    
    void begLine(int &y, int &x);
};
}

#endif
