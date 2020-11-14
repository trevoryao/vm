#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ncurses.h>

namespace ui {
class Window {
private:
    WINDOW *window;
public:
    Window(int height, int width, int x, int y) noexcept;
    Window(const Window &other) = delete;
    Window(Window &&other);
    
    Window &operator=(const Window &other) = delete;
    Window &operator=(Window &&other);
    
    explicit operator bool() const;
    
    WINDOW *get() const noexcept;
    
    ~Window();
};
}

#endif
