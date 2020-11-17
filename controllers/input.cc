#include "input.h"

#include <ncurses.h>

#include "action.h"

namespace controllers {
Action Input::action() {
    switch (getch()) {
        case 'q': return QUIT;
        case KEY_RESIZE: return RESIZE;
        /*
        case KEY_MOUSE: {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) {
                    
                } else if (event.bstate & BUTTON5_PRESSED) {
                    
                }
            }
            break;
        }*/
        default: return NONE;
    }
}
}
