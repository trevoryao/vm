#ifndef __FILE_OP_H__
#define __FILE_OP_H__

#include "i-action.h"

namespace actions {
enum class FileOpType {
    WRITE,
    WRITE_QUIT,
    QUIT,
    FORCE_QUIT,
    INSERT
};

class FileOp final : public IAction<FileOpType> {
public:
    FileOp(FileOpType value);
};
}

#endif
