#include "graphics.h"

#include "colours.h"

#include <ncurses.h>

namespace ui {
Graphics::Graphics() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    ESCDELAY = 250;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    start_color();
    use_default_colors();
    init_color(COLOR_WHITE, 1000, 1000, 1000); // brighter white
    init_pair(DEFAULT, DEFAULT_COLOUR, DEFAULT_COLOUR); // default text;
    init_pair(WARNING, COLOR_WHITE, COLOR_RED); // warnings
    init_color(FILL_COLOUR, 820, 576, 902);
    init_pair(FILL, FILL_COLOUR, DEFAULT_COLOUR);
}

Graphics::~Graphics() { endwin(); }
}
