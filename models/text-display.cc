#include "text-display.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "../ui/file.h"
#include "../views/text-view.h"

using namespace std;

namespace vm::models {
TextDisplay::TextDisplay(const string &fileName) {
    fstream fs{fileName};
    ui::File f{fs};
    text = f.read();
    
    addView(make_unique<views::TextView>(*this));
}

const vector<string> &TextDisplay::getText() { return text; }

void TextDisplay::run() {
    while (true) {
        displayViews();
    }
}
}
