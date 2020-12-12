#include "text-view.h"

#include <ncurses.h>
#include <string>

#include "../models/text-model.h"
#include "../ui/window.h"
#include "view-base.h"

using namespace models;

namespace views {
TextView::TextView(TextModel &instance) : ViewBase{getmaxy(stdscr) - 1, getmaxx(stdscr), 0, 0}, 
    instance{instance}, syntax{instance.getText(), window} { }

void TextView::update(char c, int y, int x) { window.writeChar(c, y, x); }
void TextView::update(const std::string &s, int y, int x) { window.writeStr(s, y, x); }

void TextView::displayView() {
    window.clear();
    int curY = 0, curX = 0, y = 0, maxY = getMaxHeight();
    instance.getCursor(curY, curX);
    
    if (instance.getText().isCpp()) {
        syntax.print(maxY);
    } else {
        for (int i = instance.getText().getTopLine(); i <= instance.getText().getBotLine(); ++i) {
            for (auto &line : instance.getText().getTextFile()[i].getRows()) {
                if (y > maxY || static_cast<size_t>(i) >= instance.getText().height()) break;
                window.writeStr(line, y++, 0);
            }
        }
        for (; y <= maxY; ++y) window.writeFill(y);
    }
    moveCursor(curY, curX);
}

void TextView::displayInfo() {
    int y, x;
    instance.getCursor(y, x);
    
    std::vector<std::string> aboutText{
        "vi: Vi IMproved - Mass",
        "a lightweight vim-like text editor",
        "",
        "version 1.0",
        "",
        "Trevor J. Yao",
        "University of Waterloo",
        "",
        "github.com/trevoryao/vm"
    };
        
    int l = (getMaxHeight() - aboutText.size()) / 2;
    for (auto &s : aboutText) {
        int c = (getMaxWidth() - s.size()) / 2;
        window.writeInfoStr(s, l++, c);
    }
    
    moveCursor(y, x);
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
    if (bracket) {
        window.writeChar(bracket, bY, bX);
        bracket = 0;
    }
    
    bY = y;
    bX = x;
    if ((bracket = instance.getText().getMatchingBracket(bY, bX))) {
        instance.getText().getWindowCursor(bY, bX);
        window.writeBracketPair(bracket, bY, bX);
    }
    instance.getText().getWindowCursor(y, x);
    window.move(y - instance.getText().getTopLine(), x); 
    window.refresh(); 
}
}