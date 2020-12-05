#include "text-model.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "mode-type.h"
#include "../actions/action.h"
#include "../actions/bad-entry.h"
#include "../actions/clear-cmd.h"
#include "../actions/e-movement.h"
#include "../actions/e-search.h"
#include "../actions/file-op.h"
#include "../actions/global.h"
#include "../actions/incomplete.h"
#include "../actions/insert.h"
#include "../actions/movement.h"
#include "../actions/replace.h"
#include "../actions/scroll.h"
#include "../actions/search.h"
#include "../actions/text-edit.h"
#include "../actions/update-cmd.h"
#include "../controllers/input.h"
#include "../controllers/key-input.h"
#include "../ui/colours.h"
#include "../views/status-view.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;

namespace {
inline bool isWordChar(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') ||
        ('0' <= c && c <= '9') || (c == '_');
}

inline bool isWSpace(char c) {
    return c == ' ' || c == '\t';
}
}

namespace models {
TextModel::TextModel(const string &fileName) : text{fileName}, 
    mode{ModeType::CMD}, curY{0}, curX{0}, runLoop{true} {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    start_color();
    use_default_colors();
    init_color(COLOR_WHITE, 1000, 1000, 1000); // brighter white
    init_pair(DEFAULT, DEFAULT_COLOUR, DEFAULT_COLOUR); // default text;
    init_pair(WARNING, COLOR_WHITE, COLOR_RED); // warnings
    init_color(FILL_COLOUR, 820, 576, 902);
    init_pair(FILL, FILL_COLOUR, DEFAULT_COLOUR);
    
    addView(make_unique<views::StatusView>(*this));
    addInputController(make_unique<controllers::Input>());
    addKeyController(make_unique<controllers::KeyInput>());
    
    // construct text based on view
    unique_ptr<views::TextView> textView = make_unique<views::TextView>(*this);
    text.resizeText(textView->getMaxHeight(), textView->getMaxWidth());
    addView(std::move(textView));
}

const string &TextModel::getName() { return text.getFileName(); }

Text &TextModel::getText() { return text; }

void TextModel::getCursor(int &y, int &x) {
    y = curY;
    x = curX;
}

void TextModel::setStaticCmd(Incomplete *a) {
    staticCmd = make_unique<Incomplete>(*a);
    updateStaticView(staticCmd->getFragment());
    moveCursor(curY, curX);
}

void TextModel::clearStaticCmd() {
    staticCmd.reset();
    clearStaticView();
    moveCursor(curY, curX);
}

void TextModel::setExecCmd(Incomplete *a) {
    execCmd = make_unique<Incomplete>(*a);
    updateExecView(execCmd->getFragment());
}

void TextModel::clearExecCmd() {
    execCmd.reset();
    clearExecView();
    moveCursor(curY, curX);
}

void TextModel::run() {
    displayViews(); // TODO: display columns and lines

    while (runLoop) {
        if (execCmd) {
            try {
                auto action = getAction(execCmd.get());
                if (action) {
                    action->execAction(*this);
                    clearExecCmd();
                }
            } catch (ClearCmd &e) { // TODO: not clearing fully
                clearExecCmd();
            } catch (BadEntry &e) {
                clearExecCmd();
                displayWarn("Invalid Command: " + e.getEntry());
            } catch (UpdateCmd &e) {
                execCmd->execAction(*this);
            }
        } else if (staticCmd) {
            try {
                auto action = getAction(staticCmd.get());
                if (action) {
                    action->execAction(*this);
                    clearStaticCmd();
                }
            } catch (ClearCmd &e) {
                clearStaticCmd();
            } catch (BadEntry &e) {
                clearStaticCmd();
                displayWarn("Invalid Command: " + e.getEntry());
            } catch (UpdateCmd &e) {
                staticCmd->execAction(*this);
            }
        } else {
            auto action = getAction(mode);
            if (action) action->execAction(*this);
        }
    }
}

void TextModel::quit() { runLoop = false; }

void TextModel::displayWarn(const std::string &m) {
    updateWarning(m);
    moveCursor(curY, curX);
}

void TextModel::resizeText(int MaxY, int MaxX) { text.resizeText(MaxY, MaxX); }

void TextModel::moveAllCursor(int y, int x) {
    // TODO: scroll
    if (y < text.getTopLine()) {
        scrollUp(text.getTopLine() - y);
    } else if (y > text.getBotLine()) {
        scrollDown(y - text.getBotLine());
    }
    
    moveCursor(y, x);
    curY = y;
    curX = x;
}

void TextModel::moveLeft(int n) {
    moveAllCursor(curY, curX - n < 0 ? 0 : curX - n);
}

void TextModel::moveRight(int n) {
    int max = text.getTextFile()[curY].size() - (mode == ModeType::CMD ? 2 : 1);
    moveAllCursor(curY, curX + n > max ? max : curX + n);
}

void TextModel::moveUp(int n) {
    // no scroll first, scroll later
    int i = curY - n < 0 ? 0 : curY - n;
    int x = text.getTextFile()[i].size() - (mode == ModeType::CMD ? 2 : 1);
    if (curX > x) {
        if (x < 0) x = 0;
        curX = x;
    }
    moveAllCursor(i, curX);
}

void TextModel::moveDown(int n) {
    int i = static_cast<size_t>(curY + n) >= text.getTextFile().size() ? 
        text.getTextFile().size() - 1 : curY + n;
    int x = text.getTextFile()[i].size() - (mode == ModeType::CMD ? 2 : 1);
    if (curX > x) {
        if (x < 0) x = 0;
        curX = x;
    }
    moveAllCursor(i, curX);
}

void TextModel::searchWordLeft(int n) {
    int found = 0;
    int y = curY;
    int x = curX;

    char t;
    char c = text.getTextFile()[y][x];
    if (c == '\n') --found;
    
    if (isWordChar(c)) {
        t = 0;
    } else if (!(isWSpace(c) || c == '\n')) {
        t = 1;
    } else {
        t = 2;
    }
    
    // empty line case
    char next = x > 0 ? text.getTextFile()[y][x - 1] :
        text.getTextFile()[y - 1][text.getTextFile()[y - 1].size() - 1];
    switch (t) {
        case 0: {
            if (!isWordChar(next)) --found;
            break;
        }
        case 1: {
        if (!(!isWordChar(next) && !(isWSpace(next) || next == '\n'))) --found;
            break;
        }
        case 2: {
            if (!isWSpace(next)) --found;
            break;
        }
    }
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is whitespace
    */
    while (y >= 0) {
        if (text.getTextFile()[y].size() == 1) {
            
            if (found + 1 == n) {
                moveAllCursor(y + 1, 0); // TODO: not moving?
                return;
            } else {
                --y;
                x = text.getTextFile()[y].size() - 2;
                t = 2;
                ++found;
            }
            continue;
        }
        
        while (x >= 0) {
            char c = text.getTextFile()[y][x];
            // skip over ws?
            switch (t) {
                case 0: {
                    if (isWordChar(c)) {
                        --x;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        t = 2;
                        if (found + 1 == n) ++found;
                    }
                    break;
                }
                case 1: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        --x;
                    } else {
                        t = 2;
                        if (found + 1 == n) ++found;
                    }
                    break;
                }
                case 2: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        --x;
                    }
                    break;
                }
                default: break;
            }
            
            // depends on if it starts at the end or at the beginning
            if (found == n) {
                if (text.getTextFile()[y][x + 1] != '\n') ++x;
                else {
                    ++y;
                    x = 0;
                }
                while (isWSpace(text.getTextFile()[y][x])) --x;
                moveAllCursor(y, x); // TODO: not moving?
                return;
            }
        }
        
        --y;
        x = text.getTextFile()[y].size() - 2;
        t = 2;
    }

    moveAllCursor(0, 0);
}

void TextModel::searchWordRight(int n) {
    int found = 0;
    size_t y = curY;
    size_t x = curX;

    char t;
    char c = text.getTextFile()[y][x];
    if (isWordChar(c)) {
        t = 0;
    } else if (!isWSpace(c)) {
        t = 1;
    } else {
        t = 2;
    }
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is whitespace
    */
    while (y < text.getTextFile().size()) {
        if (text.getTextFile()[y].size() == 0) { // does nothing
            ++found;
            ++y;
            if (found == n) {
                moveAllCursor(y, x);
                break;
            }
        }
        
        while (x < text.getTextFile()[y].size()) {
            char c = text.getTextFile()[y][x];
            // skip over ws?
            switch (t) {
                case 0: {
                    if (isWordChar(c)) {
                        ++x;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        t = 2;
                    }
                    break;
                }
                case 1: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++x;
                    } else {
                        t = 2;
                    }
                    break;
                }
                case 2: {
                    if (isWordChar(c)) {
                        ++found;
                        t = 0;
                    } else if (!isWSpace(c)) {
                        ++found;
                        t = 1;
                    } else {
                        ++x;
                    }
                    break;
                }
                default: break;
            }
            
            if (found == n) {
                moveAllCursor(y, x);
                return;
            }
        }
        
        ++y;
        x = 0;
        t = 2;
    }
}

void TextModel::getFirstChar() {
    for (size_t i = 0; i < text.getTextFile()[curY].size(); ++i) {
        if (!isWSpace(text.getTextFile()[curY][i])) {
            moveAllCursor(curY, i);
            return;
        }
    }
}

void TextModel::getLastChar(int n) {
    --n;
    for (size_t i = text.getTextFile()[curY - n].size() - 1; i != 0; --i) {
        if (!isWSpace(text.getTextFile()[curY - n][i])) {
            moveAllCursor(curY - n, i);
            return;
        }
    }
}

void TextModel::displayName() {
    writeMessage("\"" + text.getFileName() + "\"");
}

void TextModel::scrollUp(int lines) {
    int newTop = text.getTopLine() - lines;
    int newBottom = text.getBotLine() - lines;
    if (newTop < 0) {
        newBottom -= newTop;
        newTop = 0;
    }
    
    text.setTopLine(newTop);
    text.setBotLine(newBottom);
    
    displayViews();
}

void TextModel::scrollDown(int lines) {
    int newTop = text.getTopLine() + lines;
    int newBottom = text.getBotLine() + lines;
    
    if (static_cast<size_t>(newBottom) >= text.getTextFile().size()) {
        int diff = text.getTextFile().size() - newBottom - 1;
        newTop += diff;
        newBottom += diff;
    }
    
    text.setTopLine(newTop);
    text.setBotLine(newBottom);
    
    
    displayViews();
}

TextModel::~TextModel() { endwin(); }
}
