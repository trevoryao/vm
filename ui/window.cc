#include "window.h"

#include <ncurses.h>
#include <string>
#include <utility>

#include "colours.h"

using namespace std;

namespace ui {
Window::Window() noexcept : window{nullptr} { }

Window::Window(int height, int width, int x, int y) noexcept {
    wrefresh(stdscr);
    window = newwin(height, width, x, y);
    keypad(window, TRUE);
    scrollok(window, false);
    wattron(window, COLOR_PAIR(DEFAULT));
}

Window::Window(Window &&other): window{other.window} { other.window = nullptr; }

Window &Window::operator=(Window &&other) {
    std::swap(window, other.window);
    return *this;
}

Window::operator bool() const { return window; }

WINDOW *Window::get() const noexcept { return window; }

void Window::writeChar(char c, int y, int x) { 
    mvwaddch(window, y, x, c);
}

void Window::writeStr(const std::string &s, int y, int x) { 
    mvwaddstr(window, y, x, s.c_str());
}

void Window::writeInfoStr(const std::string &s, int y, int x) { 
    wattron(window, COLOR_PAIR(PANTONE_BLUE));
    writeStr(s, y, x);
    wattroff(window, COLOR_PAIR(PANTONE_BLUE));
}

void Window::writePurple(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(PURPLE));
}

void Window::writeOrange(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(ORANGE));
}

void Window::writeGreen(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(GREEN));
}

void Window::writeEmphGreen(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(EMPH_GREEN));
}

void Window::writeBlue(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(BLUE));
}

void Window::writeYellow(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(YELLOW));
}

void Window::writeTurq(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(TURQ));
}

void Window::writeBracketPair(char c, int y, int x) {
    mvwaddch(window, y, x, c | COLOR_PAIR(EMPH_BRACKET));
}

void Window::writeFill(int y) {
    mvwaddch(window, y, 0, '~' | COLOR_PAIR(FILL));
}

void Window::writeWarning(const std::string &s, int y, int x) { 
    wattron(window, COLOR_PAIR(WARNING));
    writeStr(s, y, x);
    wattroff(window, COLOR_PAIR(WARNING));
}

void Window::writeMode(const std::string &s, int y, int x) {
    wattron(window, A_BOLD);
    writeStr(s, y, x);
    wattroff(window, A_BOLD);
}

void Window::refresh() { wrefresh(window); }

void Window::clear() { wclear(window); }

void Window::clear(int y, int x) { writeChar(' ', y, x); }

void Window::move(int y, int x) { wmove(window, y, x); }

// may have to rewrite to delete and make new
void Window::resize(int rows, int cols, int x, int y) { 
    // wresize(window, y, x);
    // wclear(window);
    if (window) {
        delwin(window);
    }
    wrefresh(stdscr);
    window = newwin(rows, cols, x, y);
    keypad(window, TRUE);
    scrollok(window, false);
}

Window::~Window() {
    if (window) {
        delwin(window);
    }
}
}
