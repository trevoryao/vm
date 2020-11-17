#ifndef __ACTION_H__
#define __ACTION_H__

namespace controllers {
enum Action { NONE, QUIT, RESIZE };
// std::pair?
enum Movement {
    LEFT, // h
    RIGHT, // l
    UP, // k
    DOWN, // j
    WORD_LEFT, // b
    WORD_RIGHT, // w
    BEG_LINE, // 0
    BEG_CH, // ^
    END_CH, // $
};
}

#endif
