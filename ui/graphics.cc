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
    init_pair(DEFAULT, DEFAULT_COLOUR, DEFAULT_COLOUR); // default text;
    init_pair(WARNING, COLOR_WHITE, COLOR_RED); // warnings
    init_color(FILL_COLOUR, 820, 576, 902);
    init_pair(FILL, FILL_COLOUR, DEFAULT_COLOUR);
    init_color(COLOUR_PURPLE, 655, 157, 647);
    init_pair(PURPLE, COLOUR_PURPLE, DEFAULT_COLOUR);
    init_color(COLOUR_ORANGE, 894, 337, 286);
    init_pair(ORANGE, COLOUR_ORANGE, DEFAULT_COLOUR);
    init_color(COLOR_GREEN, 314, 631, 310);
    init_pair(GREEN, COLOR_GREEN, DEFAULT_COLOUR);
    init_color(COLOR_BLUE, 251, 471, 949);
    init_pair(BLUE, COLOR_BLUE, DEFAULT_COLOUR);
    init_color(COLOR_YELLOW, 757, 518, 4);
    init_pair(YELLOW, COLOR_YELLOW, DEFAULT_COLOUR);
    init_color(COLOUR_TURQ, 537, 867, 1000);
    init_pair(TURQ, COLOUR_TURQ, DEFAULT_COLOUR);
    init_color(COLOUR_EMPH_GREEN, 24, 349, 24);
    init_pair(EMPH_GREEN, COLOUR_EMPH_GREEN, DEFAULT_COLOUR);
}

Graphics::~Graphics() { endwin(); }
}
