#ifndef __DISPLAY_MESSAGE_H__
#define __DISPLAY_MESSAGE_H__

#include <string>

namespace exceptions {
class DisplayMessage final {
private:
    std::string message;
public:
    explicit DisplayMessage(const std::string &message);
    explicit DisplayMessage(std::string &&message);
    
    const std::string &getMessage();
};
}


#endif
