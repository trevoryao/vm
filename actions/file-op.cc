#include "file-op.h"

#include "../models/text-model.h"

namespace actions {
FileOp::FileOp(FileOpType value) : IAction{value} { }

void FileOp::execAction(models::TextModel &t) {
    switch (getValue()) {
        case FileOpType::WRITE: t.getText().write(); break;
        case FileOpType::WRITE_QUIT: t.getText().write(); 
        case FileOpType::FORCE_QUIT: t.quit(); break;
        case FileOpType::QUIT:
            if (t.getText().diff()) t.quit();
            else t.displayWarn("No write since last change (add ! to override)");
            break;
        case FileOpType::INSERT: break; // TODO
    }
}
}
