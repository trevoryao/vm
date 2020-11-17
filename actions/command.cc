#include "command.h"

namespace actions {
Command::Command(CommandType cmd): Action{CMDS}, cmd{cmd} { }

CommandType Command::getCmd() { return cmd; }
}
