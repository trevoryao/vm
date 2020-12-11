#ifndef __TEXT_SEARCH_H__
#define __TEXT_SEARCH_H__

#include <memory>
#include <string>

#include "../actions/e-search.h"
#include "../actions/search.h"

namespace models {
class Text;

class TextSearch final {
private:
    Text &text;
    std::unique_ptr<actions::ESearch> lastESearch;
    std::unique_ptr<actions::Search> lastSearch;
public:
    explicit TextSearch(Text &text);

    void setESearch(std::unique_ptr<actions::ESearch> eSearch);
    actions::ESearch *getESearch();

    void setSearch(std::unique_ptr<actions::Search> search);
    actions::Search *getSearch();

    void searchFwd(int &y, int &x, char c, int n);
    bool searchFwd(int &y, int &x, const std::string &s, int n, int &cycles);
    
    void searchBwd(int &y, int &x, char c, int n);
    bool searchBwd(int &y, int &x, const std::string &s, int n, int &cycles);
};
}

#endif
