#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "model-base.h"
#include "text.h"

enum class ModeType;

namespace actions {
class EMovement;
class ESearch;
class FileOp;
class Global;
class Incomplete;
class Insert;
class Keyboard;
class Movement;
class Replace;
class Scroll;
class Search;
class TextEdit;
}

namespace models {
class TextModel final : ModelBase {
private:
    std::string fileName;
    Text text;
    ModeType mode;
    int topLine, botLine, curY, curX, maxY, maxX;
    bool runLoop;
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
    void execAction(actions::Global *a);
    void execAction(actions::Incomplete *a);
    void execAction(actions::Movement *a);

    void moveLeft(int n);
    void moveRight(int n);
    void moveUp(int n);
    void moveDown(int n);
    
    void searchWordLeft(int n);
    void searchWordRight(int n);
};
}

#endif
