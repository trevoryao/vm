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
/*
void TextView::displayView() {
    int y = 0, x = 0;
    int max_y = getMaxHeight();
    for (auto &s : instance.getText().getTextFile()) { // change to reg loop starting at topLine to botLine
        if (y > max_y) break;
        window.writeStr(s, y++, x);
    }
    
    while (y <= max_y) window.writeChar('~', y++, x);
    window.move(0, 0);
    window.refresh();
}*/

void TextView::displayView() {
    int curY = 0, curX = 0, y = 0, maxY = getMaxHeight();
    instance.getCursor(curY, curX);
    
    for (int i = instance.getText().getTopLine(); i <= instance.getText().getBotLine(); ++i, ++y) {
        if (y > maxY || static_cast<size_t>(i) >= instance.getText().getTextFile().size()) break;
        window.writeStr(instance.getText().getTextFile()[i], y, 0);
    }
    for (; y <= maxY; ++y) window.writeFill(y);
    
    moveCursor(curY, curX);
}

void TextView::resizeView() {
    int y, x;
    getmaxyx(stdscr, y, x);
    window.resize(y - 1, x, 0, 0);
    instance.resizeText(y, x);
    displayView();
}

int TextView::getMaxHeight() { return getmaxy(window.get()); }
int TextView::getMaxWidth() { return getmaxx(window.get()); }

void TextView::moveCursor(int y, int x) { 
    window.move(y - instance.getText().getTopLine(), x); 
    window.refresh(); 
}
}