#include "text-view.h"

#include <ncurses.h>
#include <string>

#include "../models/text-display.h"
#include "../ui/window.h"
#include "view-base.h"

#include <iostream>

using namespace models;

namespace views {
TextView::TextView(TextDisplay &instance): window{}, instance{instance} {
    int row, col;
    getmaxyx(stdscr, row, col);
    window = ui::Window{row - 2, col - 1, 0, 0};
}

void TextView::update(char c) { window.writeChar(c); }
void TextView::update(char c, int y, int x) { window.writeChar(c, y, x); }
void TextView::update(const std::string &s) { window.writeStr(s); }
void TextView::update(const std::string &s, int y, int x) { window.writeStr(s, y, x); }

void TextView::displayView() {
    int y = 0, x = 0;
    for (auto &s : instance.getText()) window.writeStr(s, y++, x);
    int max_y = getmaxy(window.get());
    while (y <= max_y) window.writeChar('~', y++, x);
    window.move(0, 0);
    window.refresh();
}

void TextView::resizeView() {
    int y, x;
    getmaxyx(stdscr, y, x);
    window.resize(y - 2, x - 1);
    instance.resizeText(x);
}

int TextView::getMaxWidth() { return getmaxx(window.get()); }
}