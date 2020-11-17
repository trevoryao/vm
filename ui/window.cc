#include "window.h"

#include <ncurses.h>
#include <string>
#include <utility>

using namespace std;

namespace ui {
Window::Window() noexcept : window{nullptr} { }

Window::Window(int height, int width, int x, int y) noexcept {
    wrefresh(stdscr);
    window = newwin(height, width, x, y);
    keypad(window, TRUE);
    scrollok(window, false);
}

Window::Window(Window &&other): window{other.window} { other.window = nullptr; }

Window &Window::operator=(Window &&other) {
    std::swap(window, other.window);
    return *this;
}

Window::operator bool() const { return window; }

WINDOW *Window::get() const noexcept { return window; }

void Window::writeChar(char c) { 
    waddch(window, c);
}
void Window::writeChar(char c, int y, int x) { 
    mvwaddch(window, y, x, c);
}

void Window::writeStr(const std::string &s) { 
    waddstr(window, s.c_str());
}

void Window::writeStr(const std::string &s, int y, int x) { 
    mvwaddstr(window, y, x, s.c_str());
}

void Window::refresh() { wrefresh(window); }

void Window::move(int y, int x) { wmove(window, y, x); }

pair<int, int> Window::getCursor() {
    int y, x;
    getyx(window, y, x);
    return make_pair(y, x);
}

void Window::resize(int y, int x) { 
    wresize(window, y, x);
    wclear(window);
}

Window::~Window() {
    if (window) {
        delwin(window);
    }
}
}
