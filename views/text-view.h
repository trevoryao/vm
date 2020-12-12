#ifndef __TEXT_VIEW_H__
#define __TEXT_VIEW_H__

#include <string>

#include "view-base.h"
#include "../models/text-model.h"
#include "../ui/syntax.h"

namespace views {
class TextView final : public ViewBase {
private:
    char bracket;
    int bY, bX;
    models::TextModel &instance;
    ui::Syntax syntax;
public:
    TextView(models::TextModel &instance);
    
    void update(char c, int y, int x) override;
    void update(const std::string &s, int y, int x) override;
    
    void displayView() override;
    void displayInfo() override;
    
    void resizeView() override;
    
    int getMaxHeight() override;
    int getMaxWidth() override;
    
    void moveCursor(int y, int x) override;
};
}

#endif
