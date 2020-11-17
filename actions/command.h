#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "action.h"
#include "action-enums.h"

namespace actions {
class Command final : public Action {
private:
    CommandType cmd;
public:
    Command(CommandType cmd);

    CommandType getCmd();
};
}

#endif
