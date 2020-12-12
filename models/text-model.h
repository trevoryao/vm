#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "clipboard.h"
#include "model-base.h"
#include "move.h"
#include "text-search.h"
#include "text.h"
#include "undo.h"

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
enum class ModeType;

class TextModel final : public ModelBase {
private:
    Text text;
    TextSearch search;
    Move move;
    Undo undo;
    Clipboard clipboard;
    ModeType mode;
    int curY, curX;
    bool runLoop, cpp;
    std::unique_ptr<Incomplete> staticCmd;
    std::unique_ptr<Incomplete> execCmd;
public:
    explicit TextModel(const std::string &fileName);
    
    Text &getText();
    TextSearch &getSearch();
    Move &getMove();
    Undo &getUndo();
    Clipboard &getClipboard();
    
    void getCursor(int &y, int &x);
    void moveAllCursor(int y, int x);
    
    void setStaticCmd(actions::Incomplete *a);
    actions::Incomplete *getStaticCmd();
    void clearStaticCmd();
    
    void setExecCmd(actions::Incomplete *a);
    void clearExecCmd();
    
    ModeType getMode();
    void setCmdMode();
    void setInsertMode();
    void setReplaceMode();
    
    void run();
    void quit();
    
    void displayAllViews();
    void resizeText(int maxY, int maxX);
    
    void displayWarn(const std::string &m);
    void displayName();
    void displayPlainMsg(const std::string &m);
};
}

#endif
