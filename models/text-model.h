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
    Text text;
    ModeType mode;
    int curY, curX;
    bool runLoop;
    std::unique_ptr<Incomplete> staticCmd;
    std::unique_ptr<Incomplete> execCmd;
public:
    explicit TextModel(const std::string &fileName);
    
    const std::string &getName();
    Text &getText();
    void getCursor(int &y, int &x);
    
    void setStaticCmd(actions::Incomplete *a);
    void clearStaticCmd();
    
    void setExecCmd(actions::Incomplete *a);
    void clearExecCmd();
    
    void run();
    void quit();
    
    void moveAllCursor(int y, int x);
    
    void displayWarn(const std::string &m);
    
    void resizeText(int maxY, int maxX);

    void moveLeft(int n);
    void moveRight(int n);
    void moveUp(int n);
    void moveDown(int n);

    void searchWordLeft(int n);
    void searchWordRight(int n);
    
    void getFirstChar();
    void getLastChar(int n);
    
    void displayName();
    
    void scrollUp(int lines);
    void scrollDown(int lines);
    
    ~TextModel();
};
}

#endif
