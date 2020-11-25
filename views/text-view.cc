#include "text-view.h"

#include <ncurses.h>
#include <string>

#include "../models/text-model.h"
#include "../ui/window.h"
#include "view-base.h"

using namespace models;

namespace views {
TextView::TextView(TextModel &instance) : ViewBase{getmaxy(stdscr) - 1, getmaxx(stdscr), 0, 0}, 
    instance{instance} { }

void TextView::update(char c, int y, int x) { window.writeChar(c, y, x); }
void TextView::update(const std::string &s, int y, int x) { window.writeStr(s, y, x); }

void TextView::displayView() {
    int y = 0, x = 0;
    int max_y = getMaxHeight();
    for (auto &s : instance.getText()) { // change to reg loop starting at topLine to botLine
        if (y > max_y) break;
        window.writeStr(s, y++, x);
    }
    while (y <= max_y) window.writeChar('~', y++, x);
    window.move(0, 0);
    window.refresh();
}

void TextView::resizeView() {
    int y, x;
    getmaxyx(stdscr, y, x);
    window.resize(y - 1, x, 0, 0);
    instance.resizeText(x);
    displayView();
    instance.setMaxY(y - 1);
    instance.setMaxX(x);
}

int TextView::getMaxHeight() { return getmaxy(window.get()); }
int TextView::getMaxWidth() { return getmaxx(window.get()); }

void TextView::moveCursor(int y, int x) { 
    window.move(y, x); 
    window.refresh(); 
}
}