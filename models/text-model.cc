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

namespace models {
TextModel::TextModel(const string &fileName) : text{fileName}, move{text},
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

Text &TextModel::getText() { return text; }
Move &TextModel::getMove() { return move; }

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

ModeType TextModel::getMode() { return mode; }

void TextModel::setCmdMode() {
    mode = ModeType::CMD;
    if (execCmd) updateExecView(execCmd->getFragment());
}

void TextModel::setInsertMode() {
    mode = ModeType::INSERT;
    clearExecView();
    writeMode("-- INSERT --");
    moveCursor(curY, curX);
}

void TextModel::setReplaceMode() {
    mode = ModeType::REPLACE;
    clearExecView();
    writeMode("-- REPLACE --");
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

void TextModel::resizeText(int maxY, int maxX) { 
    text.resizeText(maxY, maxX);
    
    switch (mode) {
        case ModeType::CMD: {
            if (execCmd) updateExecView(execCmd->getFragment());
            if (staticCmd) updateStaticView(staticCmd->getFragment());
            break;
        }
        case ModeType::INSERT: writeMode("-- INSERT --"); break;
        case ModeType::REPLACE: writeMode("-- REPLACE --"); break;
    }
}

void TextModel::moveAllCursor(int y, int x) {
    // TODO: scroll
    if (y < text.getTopLine()) {
        text.scrollUp(text.getTopLine() - y);
        displayViews();
    } else if (y > text.getBotLine()) {
        text.scrollDown(y - text.getBotLine());
        displayViews();
    }
    
    moveCursor(y, x);
    curY = y;
    curX = x;
}

void TextModel::displayName() {
    writeMessage("\"" + text.getFileName() + "\" " + 
        to_string(text.getTextFile().size()) + " lines");
}

TextModel::~TextModel() { endwin(); }
}
