#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__

#include <string>
#include <vector>

#include "model-base.h"

namespace models {
class TextDisplay final : ModelBase {
private:
    std::vector<std::string> text;
public:
    explicit TextDisplay(const std::string &fileName);
    
    const std::vector<std::string> &getText();
    
    void run();
    
    ~TextDisplay();
};
}

#endif
