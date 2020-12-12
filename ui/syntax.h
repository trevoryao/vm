#ifndef __SYNTAX_H__
#define __SYNTAX_H__

#include <string>
#include <unordered_set>

#include "window.h"
#include "../models/row.h"
#include "../models/text.h"

namespace ui {
class Syntax final {
private:
    models::Text &t;
    Window &w;
    const std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> ids;
public:
    Syntax(models::Text &t, Window &w);

    void print(size_t maxY);
    
private:
    void checkBounds(size_t &curY, size_t &curX);
};
}

#endif
