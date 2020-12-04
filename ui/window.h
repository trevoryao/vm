#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ncurses.h>
#include <string>
#include <utility>

namespace ui {
class Window {
private:
    WINDOW *window;
public:
    Window() noexcept;
    Window(int height, int width, int y, int x) noexcept;
    Window(const Window &other) = delete;
    Window(Window &&other);
    
    Window &operator=(const Window &other) = delete;
    Window &operator=(Window &&other);
    
    explicit operator bool() const;
    
    WINDOW *get() const noexcept;
    
    // void clear();
    
    // TODO: keywords (colour)
    void writeChar(char c);
    void writeChar(char c, int y, int x);
    void writeStr(const std::string &s);
    void writeStr(const std::string &s, int y, int x);
    void writeWarning(const std::string &s, int y, int x);
    
    void refresh();
    
    void clear();
    void clear(int y, int x);
    
    void move(int y, int x);
    
    std::pair<int, int> getCursor();
    
    void resize(int rows, int cols, int y, int x);
    
    ~Window();
};
}

#endif
