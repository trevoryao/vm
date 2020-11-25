#include "status-view.h"

#include <ncurses.h>
#include <string>

#include "../models/text-model.h"
#include "../ui/window.h"
#include "view-base.h"

using namespace models;

// attach cursor coordinates to corner!
namespace views {
StatusView::StatusView(TextModel &instance): ViewBase{1, getmaxx(stdscr),
    getmaxy(stdscr) - 1, 0}, hasName{false}, instance{instance} { }

void StatusView::update(char c, int y, int x) { }
void StatusView::update(const std::string &s, int y, int x) { }

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

void StatusView::moveCursor(int y, int x) {
    window.clear();
    std::string row = std::to_string(y + 1);
    std::string col = instance.getText()[y].size() == 1 ? "0-1" : std::to_string(x + 1);
    
    window.writeStr(col, 0, getMaxWidth() - col.size());
    window.writeChar(',', 0, getMaxWidth() - col.size() - 1);
    window.writeStr(row, 0, getMaxWidth() - col.size() - row.size() - 1);
    window.refresh(); 
}
}
