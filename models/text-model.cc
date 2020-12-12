#include "text-model.h"

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "mode-type.h"
#include "../actions/action.h"
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
#include "../controllers/key-input.h"
#include "../exceptions/bad-entry.h"
#include "../exceptions/clear-cmd.h"
#include "../exceptions/display-message.h"
#include "../exceptions/display-warning.h"
#include "../exceptions/update-cmd.h"
#include "../ui/colours.h"
#include "../views/status-view.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace exceptions;
using namespace std;

namespace models {
TextModel::TextModel(const string &fileName) :
    text{fileName, getmaxy(stdscr), getmaxx(stdscr)}, search{text}, move{text},
    mode{ModeType::CMD}, curY{0}, curX{0}, runLoop{true} {

    addView(make_unique<views::StatusView>(*this));
    addInputController(make_unique<controllers::Input>());
    addKeyController(make_unique<controllers::KeyInput>());

    unique_ptr<views::TextView> textView = make_unique<views::TextView>(*this);
    addView(std::move(textView));
}

Text &TextModel::getText() { return text; }
TextSearch &TextModel::getSearch() { return search; }
Move &TextModel::getMove() { return move; }
Undo &TextModel::getUndo() { return undo; }
Clipboard &TextModel::getClipboard() { return clipboard; }

void TextModel::getCursor(int &y, int &x) {
    y = curY;
    x = curX;
}

void TextModel::setStaticCmd(Incomplete *a) {
    staticCmd = make_unique<Incomplete>(*a);
    updateStaticView(staticCmd->getStaticFragment());
    moveCursor(curY, curX);
}

Incomplete *TextModel::getStaticCmd() { return staticCmd.get(); }

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
    if (text.hasFile()) {
        writeMessage("\"" + text.getFileName() + "\" " +
            std::to_string(text.getLines()) + "L, " + std::to_string(text.getChars()) + "C");
    }
    displayViews();
    if (!text.hasFile()) displayInfo();
    moveAllCursor(curY, curX);

    while (runLoop) {
        size_t height = text.height();
        try {
            if (execCmd) {
                auto action = getAction(execCmd.get());
                if (action) {
                    action->execAction(*this);
                    clearExecCmd();
                }
            }
            else if (staticCmd) {
                auto action = getAction(staticCmd.get());
                if (action) {
                    action->execAction(*this);
                }
            }
            else {
                auto action = getAction(mode);
                if (action) action->execAction(*this);
            }
        } catch (ClearCmd &e) {
            if (execCmd) clearExecCmd();
            if (staticCmd) clearStaticCmd();
        } catch (BadEntry &e) {
            clearExecCmd();
            displayWarn("Invalid Command: " + e.getEntry());
        } catch (UpdateCmd &e) {
            execCmd->execAction(*this);
        } catch (DisplayWarning &e) {
            clearExecCmd();
            displayWarn(e.getWarning());
        } catch (DisplayMessage &e) {
            displayPlainMsg(e.getMessage());
        }

        size_t newHeight = text.height();
        size_t winHeight = getHeight() - 1;
        if (height != newHeight && text.getBotLine() < static_cast<int>(winHeight)) {
            text.setBotLine(text.getBotLine() + newHeight - height > winHeight - 1 ?
                winHeight - 1 : text.getBotLine() + newHeight - height);
            displayViews();
            moveAllCursor(curY, curX);
        }
    }
}

void TextModel::quit() { runLoop = false; }

void TextModel::displayWarn(const std::string &m) {
    updateWarning(m);
    moveCursor(curY, curX);
}

void TextModel::displayAllViews() {
    displayViews();

    switch (mode) {
        case ModeType::CMD: {
            clearExecCmd();
            if (execCmd) updateExecView(execCmd->getFragment());
            if (staticCmd) updateStaticView(staticCmd->getFragment());
            break;
        }
        case ModeType::INSERT: writeMode("-- INSERT --"); break;
        case ModeType::REPLACE: writeMode("-- REPLACE --"); break;
    }

    moveAllCursor(curY, curX);
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

    moveAllCursor(curY, curX);
}

void TextModel::moveAllCursor(int y, int x) {
    if (y < text.getTopLine()) {
        text.scrollUp(text.getTopLine() - y);
        displayAllViews();
    }
    else if (y > text.getBotLine()) {
        text.scrollDown(y - text.getBotLine());
        displayAllViews();
    }

    moveCursor(y, x);
    curY = y;
    curX = x;
}

void TextModel::displayName() {
    clearExecView();
    writeMessage("\"" + text.getFileName() + "\" " +
        to_string(text.getTextFile().size()) + " lines");
    moveCursor(curY, curX);
}

void TextModel::displayPlainMsg(const std::string &m) {
    clearExecView();
    updateExecView(m);
    moveCursor(curY, curX);
}
}
