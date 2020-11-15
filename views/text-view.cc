#include "text-view.h"

#include <string>

#include "../models/text-display.h"
#include "../ui/window.h"
#include "view-base.h"

using namespace vm::models;

namespace vm::views {
TextView::TextView(TextDisplay &instance): window{}, instance{instance} {
    // TODO: display initial text
    int row, col;
    getmaxyx(stdscr, row, col);
    window = ui::Window{row - 1, col, 0, 0};
}

void TextView::update(char c) { window.writeChar(c); }
void TextView::update(char c, int y, int x) { window.writeChar(c, y, x); }
void TextView::update(const std::string &s) { window.writeStr(s); }
void TextView::update(const std::string &s, int y, int x) { window.writeStr(s, y, x); }

void TextView::displayView() {
    for (auto &s : instance.getText()) window.writeStr(s);
}
}