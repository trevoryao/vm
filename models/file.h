#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <vector>

namespace models {
class FileDNE final { };

class File final {
private:
    std::string fileName;
public:
    explicit File(const std::string &fileName);

    const std::string &getName();

    std::string read();

    void write(const std::vector<std::string> &tmpFile);
    
    bool diff(const std::vector<std::string> &tmpFile);
};
}

#endif
