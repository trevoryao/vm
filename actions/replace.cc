#include "replace.h"

#include <memory>
#include <string>

#include "../models/text-model.h"

using namespace models;

namespace actions {
Replace::Replace(ReplaceType value, char c, int n) : IAction{value, n}, ins{c} { }

void Replace::execAction(TextModel &t) {
    switch (getValue()) {
        case ReplaceType::RPL_UNDER: {
            int y, x;
            t.getCursor(y, x);
            
            std::string s;
            
            for (int i = 0; i < getMult(); ++i) {
                if (static_cast<size_t>(x + i) >= t.getText().getTextFile()[y].size() - 1) break;
                s.push_back(t.getText().replaceChar(ins, y, x + i));
            }
            
            buffer.emplace_back(s, t.getText().getWidth());
            
            t.clearStaticCmd();
            t.moveAllCursor(y, x + s.size());
            t.displayAllViews();
            t.getUndo().addRegister(Register{y, x, std::make_unique<Replace>(*this)});
            break;
        }
        case ReplaceType::RPL: {
            t.setReplaceMode();
            break;
        }
    }
}

void Replace::undoAction(TextModel &t, int y, int x) {
    for (size_t i = 0; i < buffer.front().size(); ++i)
        t.getText().replaceChar(buffer.front()[i], y, x + i);
    
    t.moveAllCursor(y, x);
    t.displayAllViews();
}
}
