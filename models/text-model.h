#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "model-base.h"
#include "move.h"
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
    Move move;
    ModeType mode;
    int curY, curX;
    bool runLoop;
    std::unique_ptr<Incomplete> staticCmd;
    std::unique_ptr<Incomplete> execCmd;
public:
    explicit TextModel(const std::string &fileName);
    
    Text &getText();
    Move &getMove();
    
    void getCursor(int &y, int &x);
    void moveAllCursor(int y, int x);
    
    void setStaticCmd(actions::Incomplete *a);
    void clearStaticCmd();
    
    void setExecCmd(actions::Incomplete *a);
    void clearExecCmd();
    
    ModeType getMode();
    void setCmdMode();
    void setInsertMode();
    void setReplaceMode();
    
    void run();
    void quit();
    
    void resizeText(int maxY, int maxX);
    
    void displayWarn(const std::string &m);
    void displayName();
    
    ~TextModel();
};
}

#endif
