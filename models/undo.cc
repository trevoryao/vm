#include "undo.h"

#include <memory>
#include <vector>
#include <utility>

#include "text-model.h"
#include "../exceptions/display-message.h"

using namespace actions;
using namespace std;

namespace models {
void Undo::addRegister(Register r) {
    addBuffer();
    registers.push_back(std::move(r));
}

void Undo::undo(TextModel &t) {
    if (registers.empty()) throw exceptions::DisplayMessage{"Already at oldest change"};
    registers.back().undoAction(t);
    registers.pop_back(); 
}

void Undo::execLastRegister(TextModel &t) { 
    if (registers.empty()) return;
    registers.back().execAction(t); 
}

void Undo::setBuffer(unique_ptr<Buffer> b) {
    addBuffer();
    textBuffer = std::move(b);
}

Buffer *Undo::getBuffer() { return textBuffer.get(); }

void Undo::addBuffer() {
    if (textBuffer) registers.emplace_back(-1, -1, std::move(textBuffer));
}

bool Undo::hasBuffer() { return textBuffer.get(); }
}
