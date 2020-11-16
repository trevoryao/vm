#include "keyboard.h"

#include <ncurses.h>

#include "action.h"

namespace controllers {
Action Keyboard::action() {
    switch (getch()) {
        case 'q': return QUIT;
        default: return INVALID;
    }
}
}
