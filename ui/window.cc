#include "window.h"

#include <ncurses.h>
#include <string>
#include <utility>

using namespace std;

namespace vm::ui {
int Window::instances = 0;

Window::Window(): window{nullptr} { }

Window::Window(int height, int width, int x, int y) {
    window = newwin(height, width, x, y);
    if (instances++ == 0) {
        initscr();
	    raw();
	    noecho();
    }
    keypad(window, TRUE);
}

Window::Window(Window &&other): window{other.window} { other.window = nullptr; }

Window &Window::operator=(Window &&other) {
    std::swap(window, other.window);
    return *this;
}

Window::operator bool() const { return window; }

WINDOW *Window::get() const { return window; }

void Window::writeChar(char c) { waddch(window, c); }
void Window::writeChar(char c, int y, int x) { mvwaddch(window, y, x, c); }

void Window::writeStr(const std::string &s) { waddstr(window, s.c_str()); }
void Window::writeStr(const std::string &s, int y, int x) { mvwaddstr(window, y, x, s.c_str()); }

Window::~Window() {
    if (window) {
        --instances;
        if (instances == 0) endwin();
        delwin(window);
    }
}
}
