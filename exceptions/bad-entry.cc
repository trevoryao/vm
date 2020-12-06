#include "bad-entry.h"

#include <utility>

namespace exceptions {
BadEntry::BadEntry() { }

BadEntry::BadEntry(const std::string &entry) : entry{entry} { }
BadEntry::BadEntry(std::string &&entry) : entry{std::move(entry)} { }

const std::string &BadEntry::getEntry() { return entry; }
}
