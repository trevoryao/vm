#include "status-view.h"

#include <ncurses.h>
#include <string>

#include "../models/text-model.h"
#include "../ui/window.h"
#include "view-base.h"

using namespace models;

// attach cursor coordinates to corner!
namespace views {
StatusView::StatusView(TextModel &instance) : ViewBase{1, getmaxx(stdscr),
    getmaxy(stdscr) - 1, 0}, hasName{false}, instance{instance}, 
    div1{getmaxx(stdscr) / 2}, div2{div1 / 2} { }

void StatusView::update(char c, int y, int x) { }
void StatusView::update(const std::string &s, int y, int x) { }

void StatusView::updateExec(const std::string &s) {
    clearExec();
    window.writeStr(s, 0, 0);
}

void StatusView::clearExec() {
    for (int x = 0; x < div1; ++x) window.clear(0, x);
}

void StatusView::updateStatic(const std::string &s) {
    clearStatic();
    window.writeInfoStr(s, 0, div2 - div1 - s.size());
}

void StatusView::clearStatic() {
    for (int x = div1; x < div2; ++x) window.clear(0, x);
}

void StatusView::writeMessage(const std::string &s) {
    clearExec();
    window.writeInfoStr(s, 0, 0);
}

void StatusView::displayView() {
    int y, x;
    instance.getCursor(y, x);
    moveCursor(y, x);
    
    if (!hasName && instance.getName().size()) {
        window.writeStr("\"" + instance.getName() + "\"", 0, 0);
        window.refresh();
        hasName = true;
    }
    window.refresh();
}

void StatusView::resizeView() {
    window.resize(1, getmaxx(stdscr) - 1, getmaxy(stdscr) - 1, 0);
    displayView();
}

int StatusView::getMaxHeight() { return getmaxy(window.get()); }
int StatusView::getMaxWidth() { return getmaxx(window.get()); }

/*
void StatusView::moveCursor(int y, int x) {
    window.clear();
    std::string row = std::to_string(y + 1);
    std::string col = instance.getText()[y].size() == 1 ? "0-1" : std::to_string(x + 1);
    
    window.writeStr(col, 0, getMaxWidth() - col.size());
    window.writeChar(',', 0, getMaxWidth() - col.size() - 1);
    window.writeStr(row, 0, getMaxWidth() - col.size() - row.size() - 1);
    window.refresh(); 
}*/

void StatusView::moveCursor(int y, int x) {
    for (int x = div2; x <= getMaxWidth(); ++x) window.clear(0, x);
    
    std::string cursor = std::to_string(y + 1) + "," + 
        ((instance.getText()[y].size() == 1) ? "0-1" : std::to_string(x + 1));

    // assume regular large enough screen
    window.writeInfoStr(cursor, 0, (getMaxWidth() - div2 - cursor.size()) / 2);
}
}
