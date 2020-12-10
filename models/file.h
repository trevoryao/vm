#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <vector>

#include "row.h"

namespace models {
class File final {
private:
    std::string fileName;
public:
    explicit File(const std::string &fileName);

    const std::string &getName();
    void setName(const std::string &newName);

    std::vector<std::string> read();

    void write(const std::vector<Row> &tmpFile);
    
    bool diff(const std::vector<Row> &tmpFile);
};
}

#endif
