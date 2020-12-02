#include "file-op.h"

#include "action.h"

namespace actions {
FileOp::FileOp(FileOpType value) : IAction{ActionType::FILE_OP, value} { }
}
