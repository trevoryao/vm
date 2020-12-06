#ifndef __FILE_OP_H__
#define __FILE_OP_H__

#include <string>

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class FileOpType {
    WRITE,
    WRITE_QUIT,
    QUIT,
    FORCE_QUIT,
    INSERT
};

class FileOp final : public IAction<FileOpType> {
private:
    std::string filePath;
public:
    FileOp(FileOpType value, const std::string &filePath = "");
    
    void execAction(models::TextModel &t) override;
};
}

#endif
