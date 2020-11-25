#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "model-base.h"
#include "../actions/action-enums.h"
#include "text.h"

namespace models {
class TextModel final : ModelBase {
private:
    std::string fileName;
    Text text;
    bool insertMode; // 0 is cmd, 1 is insert
    int topLine, botLine, curY, curX, maxY, maxX;
public:
    explicit TextModel(const std::string &fileName);
    
    const std::string &getName();
    const std::vector<std::string> &getText();
    void getCursor(int &y, int &x);
    
    void setMaxY(int y);
    void setMaxX(int x);
    
    void run();
    
    void resizeText(int maxX);

    ~TextModel();
private:
    void move(actions::MovementType movement);
};
}

#endif
