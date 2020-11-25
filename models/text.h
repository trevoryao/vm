#ifndef __TEXT_H__
#define __TEXT_H__

#include <vector>
#include <string>

#include "file.h"

namespace models {
class Text {
private:
    int topLine, botLine;
    // File file;
    std::vector<std::string> text;
public:
    explicit Text(const std::string &fileName);
    Text(const std::string &fileName, int maxX);
    const std::vector<std::string> &getText();
    void resizeText(int maxX);
};
}

#endif
