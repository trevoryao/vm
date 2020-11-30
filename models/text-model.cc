#include "text-model.h"

#include <csignal>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "../actions/i-action.h"
#include "../actions/command.h"
#include "../actions/movement.h"
#include "../controllers/input.h"
// #include "../views/mode-view.h"
#include "../views/status-view.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;

namespace models {
TextModel::TextModel(const string &fileName) : fileName{fileName}, text{fileName}, insertMode{false}, topLine{0}, curY{0}, curX{0} {
    initscr();
	raw();
	noecho();
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    addView(make_unique<views::StatusView>(*this));
    addController(make_unique<controllers::Input>());
    
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

void TextModel::run() {
    displayViews();
    
    while (true) {
        auto action = getAction(insertMode);
        switch (action->getType()) {
            case ActionType::RESIZE: resizeViews(); break;
            case ActionType::MVT: this->move(dynamic_cast<Movement *>(action.get())->getMvt()); break;
            case ActionType::CMDS: { //execCmd(dynamic_cast<Command *>(action.get())->getCmd()) 
                if (dynamic_cast<Command *>(action.get())->getCmd() == QUIT) return;
            }
            default: break;
        }
    }
}

void TextModel::resizeText(int newMaxX) { text.resizeText(newMaxX); }

void TextModel::move(MovementType movement) {
    /*switch (movement) {
        case LEFT: if (curX) moveCursor(curY, --curX); break;
        case RIGHT: {
            if (curX < static_cast<int>(text[curY].size()) - (insertMode ? 1 : 2)) {
                moveCursor(curY, ++curX);
            }
            break;
        }
        case UP: {
            if (topLine < curY) {
                int x = text[curY - 1].size() - (insertMode ? 1 : 2);
                if (curX > x) {
                    if (x < 0) x = 0;
                    curX = x;
                }
                moveCursor(--curY, curX);
            }
            break;
        } 
        case DOWN: {
            if (curY < botLine) {
                int x = text[curY + 1].size() - (insertMode ? 1 : 2);
                if (curX > x) {
                    if (x < 0) x = 0;
                    curX = x;
                }
                moveCursor(++curY, curX);
            }
            break;
        }
        // if (curY < botLine) moveCursor(++curY, curX); break;
        default: break;
    }*/
}

TextModel::~TextModel() { endwin(); }
}
