#include "text-display.h"

#include <csignal>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "../actions/action.h"
#include "../controllers/input.h"
#include "../ui/file.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;

namespace models {
TextDisplay::TextDisplay(const string &fileName) {
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
    int maxWidth = textView->getMaxWidth();
    string line;
    line.reserve(maxWidth);
    int curWidth = 0;
    for (size_t i = 0; i < file.size(); ++i, ++curWidth) {
        if (curWidth == maxWidth) {
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
    
    addView(move(textView));
    addController(make_unique<controllers::Input>());
}

const vector<string> &TextDisplay::getText() { return text; }

void TextDisplay::run() {
    displayViews();
    
    while (true) {
        auto action = getAction();
        switch (action.getType()) {
            case ActionType::RESIZE: resizeViews(); break;
            case ActionType::MVT: this->move(dynamic_cast<Movement *>(action.get())->getMvt());
            case ActionType::CMDS: { //execCmd(dynamic_cast<Command *>(action.get())->getCmd()) 
                if (dynamic_cast<Command *>(action.get())->getCmd() == QUIT) return;
            }
            default: break;
        }
    }
}

void TextDisplay::resizeText(int maxX) {
    vector<string> newText;
    
    int curX = 0;
    string line;
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
    
}

TextDisplay::~TextDisplay() { endwin(); }
}
