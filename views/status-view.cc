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
    div1{getmaxx(stdscr) / 2}, div2{div1 + (div1 / 2)} { }

void StatusView::update(char c, int y, int x) { }
void StatusView::update(const std::string &s, int y, int x) { }

void StatusView::updateExec(const std::string &s) {
    clearExec();
    window.writeStr(s, 0, 0);
    window.refresh();
}

void StatusView::clearExec() {
    for (int x = 0; x < div1; ++x) window.clear(0, x);
    window.refresh();
}

void StatusView::updateStatic(const std::string &s) {
    clearStatic();
    window.writeStr(s, 0, div1 + (div2 - div1 - s.size()));
    window.refresh();
}

void StatusView::clearStatic() {
    for (int x = div1; x < div2; ++x) window.clear(0, x);
    window.refresh();
}

void StatusView::writeMessage(const std::string &s) {
    clearExec();
    window.writeStr(s, 0, 0);
    window.refresh();
}

void StatusView::writeWarning(const std::string &s) {
    clearExec();
    window.writeWarning(s, 0, 0);
    window.refresh();
}

void StatusView::displayView() {
    int y, x;
    instance.getCursor(y, x);

    if (!hasName && instance.getName().size()) {
        window.writeStr("\"" + instance.getName() + "\"", 0, 0);
        hasName = true;
    }

    moveCursor(y, x);
}

void StatusView::resizeView() {
    window.resize(1, getmaxx(stdscr) - 1, getmaxy(stdscr) - 1, 0);
    div1 = getmaxx(stdscr);
    div2 = div1 + (div1 / 2);
    displayView();
}

int StatusView::getMaxHeight() { return getmaxy(window.get()); }
int StatusView::getMaxWidth() { return getmaxx(window.get()); }

void StatusView::moveCursor(int y, int x) {
    for (int x = div2; x <= getMaxWidth(); ++x) window.clear(0, x);
    
    std::string cursor = std::to_string(y + 1) + "," + 
        ((instance.getText().getTextFile()[y].size() == 1) ? "0-1" : std::to_string(x + 1));

    // assume regular large enough screen
    window.writeStr(cursor, 0, div2 + (getMaxWidth() - div2 - cursor.size()) / 2);
    window.refresh();
}
}
