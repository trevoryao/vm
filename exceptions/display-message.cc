#include "display-message.h"

#include <string>
#include <utility>

namespace exceptions {
DisplayMessage::DisplayMessage(const std::string &message) : message{message} { }

DisplayMessage::DisplayMessage(std::string &&message) : message{std::move(message)} { }

const std::string &DisplayMessage::getMessage() { return message; }
}