#ifndef __BAD_ENTRY_H__
#define __BAD_ENTRY_H__

#include <iostream>
#include <string>

namespace actions {
class BadEntry {
private:
    std::string entry;
public:
    BadEntry();
    BadEntry(const std::string &entry);

    const std::string &getEntry();
};
}

std::ostream &operator<<(std::ostream &out, actions::BadEntry &b);

#endif
