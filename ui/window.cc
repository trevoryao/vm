#include "window.h"

#include <ncurses.h>
#include <utility>

namespace ui {
Window::Window(int height, int width, int x, int y) {
    window = newwin(height, width, x, y);
}

Window::Window(Window &&other): window{other.window} { other.window = nullptr; }

Window &Window::operator=(Window &&other) {
    std::swap(window, other.window);
    return *this;
}

Window::operator bool() const { return window; }

WINDOW *Window::get() const { return window; }

Window::~Window() {
    wrefresh(window);
    delwin(window);
}
}
