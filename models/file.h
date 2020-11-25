#ifndef __FILE_H__
#define __FILE_H__

#include <fstream>
#include <string>
#include <vector>

namespace models {
class File {
private:
    std::fstream &f;
public:
    explicit File(std::fstream &f);

    std::string read();

    void write(const std::vector<std::string> &tmpFile);
};
}

#endif
