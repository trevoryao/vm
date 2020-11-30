#include "bad-entry.h"

namespace actions{
BadEntry::BadEntry() { }

BadEntry::BadEntry(const std::string &entry) : entry{entry} { }

const std::string &BadEntry::getEntry() { return entry; }
}

std::ostream &operator<<(std::ostream &out, actions::BadEntry &b) {
    std::cout << b.getEntry();
    return out;
}
