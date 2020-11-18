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
    
    void update(char c) override;
    void update(char c, int y, int x) override;
    void update(const std::string &s) override;
    void update(const std::string &s, int y, int x) override;
    
    void displayView() override;
    
    void resizeView() override;
    
    int getMaxHeight() override;
    int getMaxWidth() override;
    
    void moveCursor(int y, int x) override;
};
}

#endif
