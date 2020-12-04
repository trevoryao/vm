#ifndef __TEXT_H__
#define __TEXT_H__

#include <vector>
#include <string>

#include "file.h"

namespace models {
class Text final {
private:
    int topLine, botLine;
    File file;
    std::vector<std::string> text;
public:
    explicit Text(const std::string &fileName);
    Text(const std::string &fileName, int maxX);
    
    const std::string &getFileName();
    const std::vector<std::string> &getTextFile();
    
    int getTopLine();
    int getBotLine();
    
    void resizeText(int maxX);
    
    void write();
    // true if files are different, false otherwise
    bool diff();
    bool insert(const std::string &filePath, int y, int maxX);
};
}

#endif
