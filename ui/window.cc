#include "window.h"

#include <ncurses.h>
#include <string>
#include <utility>

using namespace std;

namespace ui {
Window::Window() noexcept : window{nullptr} { }

Window::Window(int height, int width, int x, int y) noexcept {
    //window = newwin(height, width, x, y);
    window = stdscr;
    keypad(window, TRUE);
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
    wrefresh(window);
}
void Window::writeChar(char c, int y, int x) { 
    mvwaddch(window, y, x, c);
    wrefresh(window);
}

void Window::writeStr(const std::string &s) { 
    waddstr(window, s.c_str());
    wrefresh(window);
}

void Window::writeStr(const std::string &s, int y, int x) { 
    mvwaddstr(window, y, x, s.c_str());
    wrefresh(window);
}

Window::~Window() {
    if (window) {
        delwin(window);
    }
}
}
