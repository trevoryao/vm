#include "file-op.h"

#include <string>
#include <utility>

#include "../exceptions/display-message.h"
#include "../exceptions/display-warning.h"
#include "../exceptions/file-dne.h"
#include "../models/text-model.h"

using namespace exceptions;

namespace actions {
FileOp::FileOp(FileOpType value, const std::string &filePath) : 
    IAction{value}, filePath{filePath} { }

void FileOp::execAction(models::TextModel &t) {
    bool newFile = false;
    
    switch (getValue()) {
        case FileOpType::WRITE:
        case FileOpType::WRITE_QUIT: {
            if (filePath.size() != 0) {
                try {
                    t.getText().setFileName(filePath);
                    t.getText().write();
                    newFile = true;
                } catch (FileDNE &e) {
                    throw DisplayWarning{"No such file: " + filePath};
                }
            }
            if (t.getText().hasFile()) t.getText().write();
            else throw DisplayWarning{"No file name"};
            if (getValue() == FileOpType::WRITE) break;
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
            if (filePath.size() == 0) filePath = t.getText().getFileName();
            if (t.getText().insertFile(filePath, y, h)) {
                t.moveAllCursor(y + h, x);
                t.displayAllViews();
                t.moveAllCursor(y, x);
            }
            break;
        }
    }
    
    if (newFile) {
        throw DisplayMessage{"\"" + filePath + "\" [new] " +
            std::to_string(t.getText().getLines()) + "L, " +
            std::to_string(t.getText().getChars()) + "C written"};
    }
}
}
