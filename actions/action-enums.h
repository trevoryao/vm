#ifndef __ACTION_ENUMS_H__
#define __ACTION_ENUMS_H__

namespace actions {
// enum Action { NONE, QUIT, RESIZE };
enum ActionType { NONE, RESIZE, MVT, CMDS };
// std::pair?
enum MovementType {
    // NONE, // null
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

enum CommandType {
    QUIT, //:q,
};
}

#endif
