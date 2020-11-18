#include "input.h"

#include <memory>
#include <ncurses.h>
#include <string>

#include "../actions/action.h"
#include "../actions/action-enums.h"
#include "../actions/command.h"
#include "../actions/movement.h"

using namespace actions;
using namespace std;

namespace controllers {
Input::Input() {
    typeMap = {
        {ERR, ActionType::NONE},
        {KEY_RESIZE, ActionType::RESIZE},
        {'h', ActionType::MVT},
        {'l', ActionType::MVT},
        {'k', ActionType::MVT},
        {'j', ActionType::MVT},
        {'b', ActionType::MVT},
        {'w', ActionType::MVT},
        {'0', ActionType::MVT},
        {'^', ActionType::MVT},
        {'$', ActionType::MVT},
        {'q', ActionType::CMDS},
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

unique_ptr<Action> Input::action() {
    int c = getch();

    if (typeMap.count(c) == 0) return make_unique<Action>();

    switch (typeMap.at(c)) {
        case RESIZE: return make_unique<Action>(RESIZE);
        case MVT: return make_unique<Movement>(mvtMap.at(c));
        case CMDS: {
            return make_unique<Command>(QUIT);
            /*
            string cmd = ":";
            int ch; // switch to buffer?
            nodelay(stdscr, false);
            while (ch = getch()) { // should rewrite this system to display on screen
                cmd.push_back(ch);
                if (cmdMap.count(ch) == 1) {
                    nodelay(stdscr, true);
                    return make_unique<Command>(cmdMap.at(cmd));
                }
            }*/
        }
        default: return make_unique<Action>();
    }
}
}
