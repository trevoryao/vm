#ifndef __BAD_ENTRY_H__
#define __BAD_ENTRY_H__

#include <iostream>
#include <string>

namespace exceptions {
class BadEntry final {
private:
    std::string entry;
public:
    BadEntry();
    explicit BadEntry(const std::string &entry);
    explicit BadEntry(std::string &&entry);

    const std::string &getEntry();
};
}

#endif
