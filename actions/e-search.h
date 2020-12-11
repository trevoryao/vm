#ifndef __E_SEARCH_H__
#define __E_SEARCH_H__

#include "i-action.h"

#include <string>

namespace models {
class TextModel;
}

namespace actions {
enum class ESearchType { 
    NEW_FWD_SEARCH,
    NEW_PREV_SEARCH,
    REPEAT,
    REPEAT_OPP
};

class ESearch final : public IAction<ESearchType> {
private:
    std::string search;
public:
    ESearch(ESearchType value, int n = 1, const std::string &search = "");
    
    const std::string &getSearch();
    
    void execAction(models::TextModel &t) override;
};
}

#endif
