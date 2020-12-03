#include "text-model.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "mode-type.h"
#include "../actions/action.h"
#include "../actions/bad-entry.h"
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
#include "../controllers/input.h"
#include "../views/status-view.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;

namespace {
inline bool isWordChar(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') ||
        ('0' <= c && '9') || (c == '_');
}

inline bool isWSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}
}

namespace models {
TextModel::TextModel(const string &fileName) : fileName{fileName}, text{fileName}, 
    mode{ModeType::CMD}, topLine{0}, curY{0}, curX{0}, runLoop{true} {
    initscr();
	raw();
	noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    addView(make_unique<views::StatusView>(*this));
    addInputController(make_unique<controllers::Input>());
    
    // construct text based on view
    unique_ptr<views::TextView> textView = make_unique<views::TextView>(*this);
    text.resizeText(textView->getMaxWidth());
    addView(std::move(textView));
}

const string &TextModel::getName() { return fileName; }

const vector<string> &TextModel::getText() { return text.getText(); }

void TextModel::getCursor(int &y, int &x) {
    y = curY;
    x = curX;
}

void TextModel::setMaxY(int y) { maxY = y; }
void TextModel::setMaxX(int x) { maxX = x; }

void TextModel::setStaticCmd(Incomplete *a) {
    staticCmd = make_unique<Incomplete>(a);
    updateStaticView(staticCmd->getFragment());
}

void TextModel::setExecCmd(Incomplete *a) {
    execCmd = make_unique<Incomplete>(a);
    updateExecView(execCmd->getFragment());
}

void TextModel::run() {
    displayViews();

    while (runLoop) {
        if (execCmd) {
            try {
                auto action = getAction(staticCmd.get());
                action->execAction(*this);
            } catch (BadEntry &e) {
                clearExecView();
            }
        } else if (staticCmd) {
            try {
                auto action = getAction(staticCmd.get());
                action->execAction(*this);
            } catch (BadEntry &e) {
                clearStaticView();
            }
        } else {
            auto action = getAction(mode);
            action->execAction(*this);
        }
    }
}

void TextModel::resizeText(int newMaxX) { text.resizeText(newMaxX); }

/*
void TextModel::execAction(Global *a) {
    switch (a->getValue()) {
        case GlobalType::NONE: break;
        case GlobalType::RESIZE: resizeViews(); break;
        case GlobalType::DISPLAY_FILE: break; // TODO
    }
}

void TextModel::execAction(Incomplete *a) {
    if (a->getValue() == IncType::EXEC) {
        execCmd.reset(a);
    } else {
        staticCmd.reset(a);
    }
}

void TextModel::execAction(Movement *a) {
    switch (a->getValue()) {
        case MvtType::LEFT: moveLeft(a->getMult()); break;
        case MvtType::RIGHT: moveRight(a->getMult()); break;
        case MvtType::UP: moveUp(a->getMult()); break;
        case MvtType::DOWN: moveDown(a->getMult()); break;
        case MvtType::WORD_LEFT: break;
        case MvtType::WORD_RIGHT: break;
        case MvtType::BEG_LINE: moveLeft(curX); break;
        case MvtType::BEG_CH: break;
        case MvtType::END_CH: break;
    }
}*/

void TextModel::moveAllCursor(int y, int x) {
    moveCursor(y, x);
    curY = y;
    curX = x;
}

void TextModel::moveLeft(int n) {
    moveAllCursor(curY, curX - n < 0 ? 0 : curX - n);
}

void TextModel::moveRight(int n) {
    int max = text.getText()[curY].size() - (mode == ModeType::CMD ? 2 : 1);
    moveAllCursor(curY, curX + n > max ? max : curX + n);
}

void TextModel::moveUp(int n) {
    // no scroll first, scroll later
     if (topLine < curY) {
        int x = text.getText()[curY - n].size() - (mode == ModeType::CMD ? 2 : 1);
        if (curX > x) {
            if (x < 0) x = 0;
            curX = x;
        }
        moveAllCursor(curY - n, curX);
    }
}

void TextModel::moveDown(int n) {
    if (curY < botLine) {
        int x = text.getText()[curY + 1].size() - (mode == ModeType::CMD ? 2 : 1);
        if (curX > x) {
            if (x < 0) x = 0;
            curX = x;
        }
        moveAllCursor(curY + n, curX);
    }
}

void TextModel::searchWordLeft(int n) {
    int found = 0;
    int y = curY;
    int x = curX;
    char t = -1;
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is empty line
    */
    while (y >= 0) {
        if (y != curY && x != curX) x = text.getText()[y].size();
        
        if (x == 0) {
            ++found;
            if (found == n) {
                moveAllCursor(y, x);
                break;
            }
        }
        
        while (x >= 0) {
            switch (t) {
                case -1: {
                    char c = text.getText()[y][x];
                    if (isWordChar(c)) t = 0;
                    if (!isWSpace(c)) t = 1;
                    --x;
                    break;
                }
                case 0: {
                    char c = text.getText()[y][x];
                    if (isWordChar(c)) --x;
                    else ++n;
                    break;
                }
                case 1: {
                    char c = text.getText()[y][x];
                    if (!isWSpace(c)) --x;
                    else ++n;
                    break;
                }
                default: break;
            }
            
            if (found == n) {
                moveAllCursor(y, x);
                break;
            }
        }
        
        --y;
    }
}

void TextModel::searchWordRight(int n) {
    int found = 0;
    int y = curY;
    int x = curX;
    char t = -1;
    
    /*
    -1 is nothing yet, 0 is word, 1 is non-blank, 2 is empty line
    */
    while (y < text.getText().size()) {
        if (y != curY && x != curX) x = 0;
        
        if (text.getText()[y].size() == 0) {
            ++found;
            if (found == n) {
                moveAllCursor(y, x);
                break;
            }
        }
        
        while (x < text.getText()[y].size()) {
            switch (t) {
                case -1: {
                    char c = text.getText()[y][x];
                    if (isWordChar(c)) t = 0;
                    if (!isWSpace(c)) t = 1;
                    ++x;
                    break;
                }
                case 0: {
                    char c = text.getText()[y][x];
                    if (isWordChar(c)) ++x;
                    else ++n;
                    break;
                }
                case 1: {
                    char c = text.getText()[y][x];
                    if (!isWSpace(c)) ++x;
                    else ++n;
                    break;
                }
                default: break;
            }
            
            if (found == n) {
                moveAllCursor(y, x);
                break;
            }
        }
        
        ++y;
    }
}

void TextModel::getFirstChar() {
    for (size_t i = 0; i < text.getText()[curY].size(); ++i) {
        if (!isWSpace(text.getText()[curY][i])) {
            moveAllCursor(curY, i);
        }
    }
}

void TextModel::getLastChar(int n) {
    --n;
    for (size_t i = text.getText()[curY - n].size() - 1; i != 0; --i) {
        if (!isWSpace(text.getText()[curY - n][i])) {
            moveAllCursor(curY - n, i);
        }
    }
}

void TextModel::displayName() {
    writeMessage(fileName);
}

TextModel::~TextModel() { endwin(); }
}
