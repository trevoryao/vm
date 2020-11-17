#include "input.h"

#include <ncurses.h>
#include <string>

#include "../actions/action.h"
#include "../actions/action-enums.h"
#include "../actions/command.h"
#include "../actions/movement.h"

using namespace std;

namespace controllers {
Input::Input() {
    typeMap = {
        {KEY_RESIZE, RESIZE},
        {'h', MVT},
        {'l', MVT},
        {'k', MVT},
        {'j', MVT},
        {'b', MVT},
        {'w', MVT},
        {'0', MVT},
        {'^', MVT},
        {'$', MVT},
        {':', CMDS},
    };
    
    mvtMap = {
        {'h', LEFT},
        {'l', RIGHT},
        {'k', UP},
        {'j', DOWN},
        {'b', WORD_LEFT},
        {'w', WORD_RIGHT},
        {'0', BEG_LINE},
        {'^', BEG_CH},
        {'$', END_CH},
    };
    
    cmdMap = {
        {"q", QUIT}
    };
}

Action Input::action() {
    int c = getch();
    
    switch (typeMap.at(c)) {
        case RESIZE: return make_unique<Action>(RESIZE);
        case MVT: return make_unique<Movement>(mvtMap.at(c));
        case CMDS: return make_unique<Command>(cmdMap.at(c));
    }
}
}
