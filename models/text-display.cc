#include "text-display.h"

#include <csignal>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "../actions/action.h"
#include "../actions/command.h"
#include "../actions/movement.h"
#include "../controllers/input.h"
#include "../ui/file.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;

namespace models {
TextDisplay::TextDisplay(const string &fileName) : insertMode{false}, topLine{0}, curY{0}, curX{0} {
    string file;
    
    if (fileName != "") {
        fstream fs{fileName};
        ui::File f{fs};
        file = f.read();
    }
    
    initscr();
	raw();
	noecho();
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    // construct text based on view
    unique_ptr<views::TextView> textView = make_unique<views::TextView>(*this);
    maxX = textView->getMaxWidth();
    maxY = textView->getMaxHeight();
    string line;
    line.reserve(maxX);
    int curWidth = 0;
    for (size_t i = 0; i < file.size(); ++i, ++curWidth) {
        if (curWidth == maxX) {
            text.push_back(line);
            line.clear();
            curWidth = 0;
        }
        
        line.push_back(file[i]);
        
        if (file[i] == '\n') {
            text.push_back(line);
            line.clear();
            curWidth = 0;
        }
    }
    botLine = maxX > static_cast<int>(text.size()) ? text.size() - 1 : maxX - 1;
    
    addView(std::move(textView));
    addController(make_unique<controllers::Input>());
}

const vector<string> &TextDisplay::getText() { return text; }

void TextDisplay::setMaxY(int y) { maxY = y; }
void TextDisplay::setMaxX(int x) { maxX = x; }

void TextDisplay::run() {
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

void TextDisplay::resizeText(int newMaxX) {
    vector<string> newText;
    
    int curX = 0;
    string line;
    maxX = newMaxX;
    line.reserve(maxX);
    for (auto &v : text) {
        for (auto c : v) {
            if (curX == maxX) {
                newText.push_back(line);
                line.clear();
                curX = 0;
            }
            
            line.push_back(c);
            ++curX;
            if (c == '\n') {
                newText.push_back(line);
                line.clear();
                curX = 0;
            }
            
        }
    }
    
    text = newText;
    displayViews();
}

void TextDisplay::move(MovementType movement) {
    switch (movement) {
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
    }
}

TextDisplay::~TextDisplay() { endwin(); }
}
