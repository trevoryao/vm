#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ncurses.h>
#include <string>

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
    
    void writeChar(char c);
    void writeChar(char c, int y, int x);
    void writeStr(const std::string &s);
    void writeStr(const std::string &s, int y, int x);
    
    void refresh();
    // TODO: keywords (colour)
    
    ~Window();
};
}

#endif
