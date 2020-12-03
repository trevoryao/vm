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
class TextModel final : public ModelBase {
private:
    std::string fileName;
    Text text;
    ModeType mode;
    int topLine, botLine, curY, curX, maxY, maxX;
    bool runLoop;
    std::unique_ptr<Incomplete> staticCmd;
    std::unique_ptr<Incomplete> execCmd;
public:
    explicit TextModel(const std::string &fileName);
    
    const std::string &getName();
    const std::vector<std::string> &getText();
    void getCursor(int &y, int &x);
    
    void setMaxY(int y);
    void setMaxX(int x);
    
    void setStaticCmd(actions::Incomplete *a);
    void setExecCmd(actions::Incomplete *a);
    
    void run();
    
    void resizeText(int maxX);

    void moveLeft(int n);
    void moveRight(int n);
    void moveUp(int n);
    void moveDown(int n);

    void searchWordLeft(int n);
    void searchWordRight(int n);
    
    void getFirstChar();
    void getLastChar(int n);
    
    void displayName();
    
    ~TextModel();
private:
    void moveAllCursor(int y, int x);
};
}

#endif
