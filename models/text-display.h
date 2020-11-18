#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "model-base.h"
#include "../actions/action-enums.h"

namespace models {
class TextDisplay final : ModelBase {
private:
    std::vector<std::string> text;
    bool insertMode; // 0 is cmd, 1 is insert
    int topLine, botLine, curY, curX, maxY, maxX;
public:
    explicit TextDisplay(const std::string &fileName);
    
    const std::vector<std::string> &getText();
    
    void setMaxY(int y);
    void setMaxX(int x);
    
    void run();
    
    void resizeText(int maxX);

    ~TextDisplay();
private:
    void move(actions::MovementType movement);
};
}

#endif
