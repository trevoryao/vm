#include "text-search.h"

#include <memory>
#include <string>
#include <utility>

#include "text.h"
#include "../actions/e-search.h"
#include "../actions/search.h"

using namespace actions;
using namespace std;

namespace models {
TextSearch::TextSearch(Text &text) : text{text} { }

void TextSearch::setESearch(unique_ptr<ESearch> eSearch) {
    lastESearch = move(eSearch);
}

ESearch *TextSearch::getESearch() { return lastESearch.get(); }

void TextSearch::setSearch(unique_ptr<Search> search) {
    lastSearch = move(search);
}

Search *TextSearch::getSearch() { return lastSearch.get(); }

void TextSearch::searchFwd(int &y, int &x, char c, int n) {
    // if reach end of line and n isnt reached, don't move
    int found = 0;
    for (size_t i = x + 1; i < text.getTextFile()[y].size() - 1; ++i) {
        if (text.getTextFile()[y][i] == c) ++found;
        if (found == n) {
            x = i;
            return;
        }
    }
}

bool TextSearch::searchFwd(int &y, int &x, const string &s, int n, int &cycles) {
    int found = 0;
    cycles = 0;
    do {
        size_t i = y, j = x + 1;
        while (i < text.getTextFile().size()) {
            size_t newPos = text.getTextFile()[i].find(s, j);
            if (newPos == string::npos) {
                ++i;
                j = 0;
            } else {
                ++found;
                if (found == n) {
                    y = i;
                    x = newPos;
                    return true;
                }
                j = newPos + 1;
            }
        }
        i = 0;
        ++cycles;
        while (i < static_cast<size_t>(y) || (i < static_cast<size_t>(y) && j <= static_cast<size_t>(x))) {
            size_t newPos = text.getTextFile()[i].find(s, j);
            if (newPos == string::npos) {
                ++i;
                j = 0;
            } else {
                ++found;
                if (found == n) {
                    y = i;
                    x = newPos;
                    return true;
                }
                j = newPos + 1;
            }
        }
    } while (found != 0);
    return false;
}

void TextSearch::searchBwd(int &y, int &x, char c, int n) {
    int found = 0;
    for (int i = x - 1; i >= 0; --i) {
        if (text.getTextFile()[y][i] == c) ++found;
        if (found == n) {
            x = i;
            return;
        }
    }
}

bool TextSearch::searchBwd(int &y, int &x, const std::string &s, int n, int &cycles) {
    int found = 0;
    cycles = 0;
    do {
        int i = y, j = x;
        while (i >= 0) {
            size_t newPos = text.getTextFile()[i].rfind(s, j);
            if (newPos == string::npos) {
                --i;
                j = text.getTextFile()[i].size() - s.size();
            } else {
                ++found;
                if (found == n) {
                    y = i;
                    x = newPos;
                    return true;
                }
                j = newPos - 1;
            }
            if (j < 0) {
                --i;
                j = text.getTextFile()[i].size() - s.size();
            }
        }
        i = text.getTextFile().size() - 1;
        ++cycles;
        while (i > y || (i == y && j >= x)) {
            size_t newPos = text.getTextFile()[i].rfind(s, j);
            if (newPos == string::npos) {
                --i;
                j = text.getTextFile()[i].size() - s.size();
            } else {
                ++found;
                if (found == n) {
                    y = i;
                    x = newPos;
                    return true;
                }
                j = newPos - 1;
            }
            if (j < 0) {
                --i;
                j = text.getTextFile()[i].size() - s.size();
            }
        }
    } while (found != 0);
    return false;
}
}
