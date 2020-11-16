#include "text-display.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "../controllers/action.h"
#include "../controllers/keyboard.h"
#include "../ui/file.h"
#include "../views/text-view.h"

using namespace controllers;
using namespace std;

namespace models {
TextDisplay::TextDisplay(const string &fileName) {
    if (fileName != "") {
        fstream fs{fileName};
        ui::File f{fs};
        text = f.read();
    }
    
    initscr();
	raw();
	noecho();
    
    addView(make_unique<views::TextView>(*this));
    addController(make_unique<controllers::Keyboard>());
}

const vector<string> &TextDisplay::getText() { return text; }

void TextDisplay::run() {
    displayViews();
    
    while (true) {
        Action action = getAction();
        switch (action) {
            case Action::QUIT: return;
            default: continue;
        }
    }
}

TextDisplay::~TextDisplay() { endwin(); }
}
