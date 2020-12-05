#include "move.h"

#include "mode-type.h"
#include "text.h"

using namespace std;

namespace {
inline bool isWordChar(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') ||
        ('0' <= c && c <= '9') || (c == '_');
}

inline bool isWSpace(char c) {
    return c == ' ' || c == '\t';
}
}

namespace models {
Move::Move(Text &text) : text{text} { }

void Move::left(int &y, int &x, int n) {
    x = x - n < 0 ? 0 : x - n;
}

void Move::right(int &y, int &x, int n, ModeType mode) {
    int max = text.getTextFile()[y].size() - (mode == ModeType::CMD ? 2 : 1);
    x = x + n > max ? max : x + n;
}

void Move::up(int &y, int &x, int n, ModeType mode) {
    // no scroll first, scroll later
    int i = y - n < 0 ? 0 : y - n;
    int max = text.getTextFile()[i].size() - (mode == ModeType::CMD ? 2 : 1);
    if (x > max) {
        if (max < 0) max = 0;
        x = max;
    }
}

void Move::down(int &y, int &x, int n, ModeType mode) {
    int i = static_cast<size_t>(y + n) >= text.getTextFile().size() ? 
        text.getTextFile().size() - 1 : y + n;
    int max = text.getTextFile()[i].size() - (mode == ModeType::CMD ? 2 : 1);
    if (x > max) {
        if (max < 0) max = 0;
        x = max;
    }
}

void Move::wordBwd(int &y, int &x, int n) {
    int found = 0;

    char t;
    char c = text.getTextFile()[y][x];
    if (c == '\n') --found;
    
    if (isWordChar(c)) {
        t = 0;
    } else if (!(isWSpace(c) || c == '\n')) {
        t = 1;
    } else {
        t = 2;
    }
    
    // empty line case
    char next = x > 0 ? text.getTextFile()[y][x - 1] :
        text.getTextFile()[y - 1][text.getTextFile()[y - 1].size() - 1];
    switch (t) {
        case 0: {
            if (!isWordChar(next)) --found;
            break;
        }
        case 1: {
        if (!(!isWordChar(next) && !(isWSpace(next) || next == '\n'))) --found;
            break;
        }
        case 2: {
            if (!isWSpace(next)) --found;
            break;
        }
    }
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is whitespace
    */
    while (y >= 0) {
        if (text.getTextFile()[y].size() == 1) {
            
            if (found + 1 == n) {
                ++y;
                x = 0;
                return;
            } else {
                --y;
                x = text.getTextFile()[y].size() - 2;
                t = 2;
                ++found;
            }
            continue;
        }
        
        while (x >= 0) {
            char c = text.getTextFile()[y][x];
            // skip over ws?
            switch (t) {
                case 0: {
                    if (isWordChar(c)) {
                        --x;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        t = 2;
                        if (found + 1 == n) ++found;
                    }
                    break;
                }
                case 1: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        --x;
                    } else {
                        t = 2;
                        if (found + 1 == n) ++found;
                    }
                    break;
                }
                case 2: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        --x;
                    }
                    break;
                }
                default: break;
            }
            
            // depends on if it starts at the end or at the beginning
            if (found == n) {
                if (text.getTextFile()[y][x + 1] != '\n') ++x;
                else {
                    ++y;
                    x = 0;
                }
                while (isWSpace(text.getTextFile()[y][x])) --x;
                return;
            }
        }
        
        --y;
        x = text.getTextFile()[y].size() - 2;
        t = 2;
    }

    y = 0;
    x = 0;
}

void Move::wordFwd(int &y, int &x, int n) {
    int found = 0;

    char t;
    char c = text.getTextFile()[y][x];
    if (isWordChar(c)) {
        t = 0;
    } else if (!isWSpace(c)) {
        t = 1;
    } else {
        t = 2;
    }
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is whitespace
    */
    while (static_cast<int>(y) < text.getTextFile().size()) {
        if (text.getTextFile()[y].size() == 0) { // does nothing
            ++found;
            ++y;
            if (found == n) return;
        }
        
        while (static_cast<int>(x) < text.getTextFile()[y].size()) {
            char c = text.getTextFile()[y][x];
            // skip over ws?
            switch (t) {
                case 0: {
                    if (isWordChar(c)) {
                        ++x;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        t = 2;
                    }
                    break;
                }
                case 1: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++x;
                    } else {
                        t = 2;
                    }
                    break;
                }
                case 2: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        ++x;
                    }
                    break;
                }
                default: break;
            }
            
            if (found == n) return;
        }
        
        ++y;
        x = 0;
        t = 2;
    }
}

void Move::firstChar(int &y, int &x, int n) {
    for (size_t i = 0; i < text.getTextFile()[y].size(); ++i) {
        if (!isWSpace(text.getTextFile()[y][i])) {
            int size = text.getTextFile()[y].size();
            x = (i + n - 1 >= size) ? size - 1 : i + n - 1;
            return;
        }
    }
}

void Move::lastChar(int &y, int &x, int lines, int n) {
    --n;
    y -= lines;
    for (size_t i = text.getTextFile()[y].size() - 1; i != 0; --i) {
        if (!isWSpace(text.getTextFile()[y][i])) {
            x = (i - n + 1 < 0) ? 0 : i - n + 1;
            return;
        }
    }
}

void Move::begLine(int &y, int &x) { x = 0; }
}
