#include "file-op.h"

#include <string>
#include <utility>

#include "../exceptions/display-warning.h"
#include "../models/text-model.h"

using namespace exceptions;

namespace actions {
FileOp::FileOp(FileOpType value, const std::string &filePath) : 
    IAction{value}, filePath{filePath} { }

void FileOp::execAction(models::TextModel &t) {
    switch (getValue()) {
        case FileOpType::WRITE: {
            if (t.getText().hasFile()) t.getText().write();
            else throw DisplayWarning{"No file name"};
            break;
        }
        case FileOpType::WRITE_QUIT: {
            if (t.getText().hasFile()) t.getText().write();
            else throw DisplayWarning{"No file name"};
        }
        case FileOpType::FORCE_QUIT: t.quit(); break;
        case FileOpType::QUIT: {
            if (t.getText().hasFile() && !t.getText().diff()) t.quit();
            else throw DisplayWarning{"No write since last change (add ! to override)"};
            break;
        }
        case FileOpType::INSERT: {
            int y, x, h;
            t.getCursor(y, x);
            if (t.getText().insert(filePath, y, h)) {
                t.moveAllCursor(y + h, x);
                t.displayAllViews();
                t.moveAllCursor(y, x);
            }
            break;
        }
    }
}
}
