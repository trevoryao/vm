#ifndef __TEXT_VIEW_H__
#define __TEXT_VIEW_H__

#include <string>

#include "../models/text-display.h"
#include "../ui/window.h"
#include "view-base.h"

namespace views {
class TextView final : public ViewBase {
private:
    ui::Window window;
    models::TextDisplay &instance;
public:
    TextView(models::TextDisplay &instance);
    
    void update(char c);
    void update(char c, int y, int x);
    void update(const std::string &s);
    void update(const std::string &s, int y, int x);
    
    void displayView() override;
};
}

#endif
