#include "syntax.h"

#include <algorithm>
#include <regex>
#include <string>
#include <vector>

#include "window.h"
#include "../models/row.h"
#include "../models/text.h"

using namespace models;
using namespace std;

namespace {
inline bool isWord(char c) {
    return ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || 
        (c == '_');
}

bool isNum(const std::string &s) {
    if (s.size() == 0) return false;
    for (auto &c : s) {
        if ('0' > c || c > '9') return false;
    }
    return true;
}
}

namespace ui {
Syntax::Syntax(Text &t, Window &w) : t{t}, w{w},
    keywords{
        "auto",
        "bool",
        "break",
        "case",
        "catch",
        "char",
        "class",
        "const",
        "constexpr",
        "const_cast",
        "continue",
        "decltype",
        "default",
        "delete",
        "do",
        "double",
        "dynamic_cast",
        "else",
        "enum",
        "explicit",
        "export",
        "extern",
        "false",
        "float",
        "for",
        "friend",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "namespace",
        "new",
        "noexcept",
        "nullptr",
        "operator"
        "private"
        "protected"
        "public",
        "register",
        "reinterpret_cast",
        "requires",
        "return",
        "short",
        "signed",
        "sizeof",
        "size_t"
        "static",
        "static_assert",
        "static_cast",
        "struct",
        "switch",
        "template",
        "this",
        "throw",
        "true",
        "try",
        "typedef",
        "typeid",
        "typename",
        "union",
        "unsigned",
        "using",
        "virtual",
        "void",
        "while",
        "std",
        "string",
        "vector",
        "list"
        "iostream",
        "ostream"
        "ifstream",
        "fstream",
        "ofstream",
        "cout",
        "cin",
        "cerr",
        "unique_ptr",
        "shared_ptr",
        "move",
        "forward",
        "endl"
    } { }

void Syntax::print(size_t maxY) {
    string buf;
    size_t y = 0, x = 0; // accessing text
    size_t curY = 0, curX = 0; // window
    // check from 0 to bolLine for identifiers
    // add to vector
    // get whitespace, check in order, check for identifier
    // search for word char, add to buffer, when no longer word char
    // check kind, print non word chars
    string preText = t.getPreText();
    regex r{"(\\w+\\s+)?(\\w+\\s+)?(\\w+(\\s+|\\s?(\\*|&)\\s?))([a-zA-Z_]\\w*)"};
    smatch m;
    ids = unordered_set<string>{sregex_token_iterator{preText.begin(), preText.end(), r, 6}, sregex_token_iterator{}};
    
    vector<string> lines = t.getFullLines();
    while (y < lines.size()) {
        auto &line = lines[y];
        while (x < line.size()) {
            char &c = line[x];
            if (isWord(c)) {
                buf.push_back(c);
                ++x;
                continue;
            }
            
            if (find(keywords.begin(), keywords.end(), buf) != keywords.end()) {
                // keyword
                for (size_t i = 0; i < buf.size(); ++i) {
                    w.writePurple(buf[i], curY, curX++);
                    checkBounds(curY, curX);
                }
            } else if (isNum(buf)) {
                // num
                for (size_t i = 0; i < buf.size(); ++i) {
                    w.writeOrange(buf[i], curY, curX++);
                    checkBounds(curY, curX);
                }
            } else if (find(ids.begin(), ids.end(), buf) != ids.end()) {
                // id
                 for (size_t i = 0; i < buf.size(); ++i) {
                    w.writeBlue(buf[i], curY, curX++);
                    checkBounds(curY, curX);
                }
            } else {
                // nothing
                 for (size_t i = 0; i < buf.size(); ++i) {
                    w.writeChar(buf[i], curY, curX++);
                    checkBounds(curY, curX);
                }
            }
            buf.clear();
            
            if (c == '\"' || c == '\'') {
                // start of string
                w.writeChar(c, curY, curX++);
                checkBounds(curY, curX);
                x++;
                size_t pos = x + 1;
                do {
                    size_t newPos = line.find(c, pos);
                    if (newPos == pos) pos = string::npos;
                    else pos = newPos;
                } while (pos != string::npos && line[pos - 1] == '\\');

                pos = line.find(c, x + 1);
                if (pos == string::npos) {
                    for (; x < line.size(); ++x) {
                        w.writeChar(line[x], curY, curX++);
                        checkBounds(curY, curX);
                    }
                    break;
                }
                for (; x < pos; ++x) {
                    if (line[x] == '\\') {
                        w.writeEmphGreen(line[x], curY, curX++);
                    } else {
                        w.writeGreen(line[x], curY, curX++);
                    }
                    checkBounds(curY, curX);
                }
                w.writeChar(c, curY, curX++);
                checkBounds(curY, curX);
                x++;
            } else if (c == '/') {
                // comment
                if (line[x + 1] == '/') {
                    for (; x < line.size(); ++x) {
                        w.writeYellow(line[x], curY, curX++);
                        checkBounds(curY, curX);
                    }
                    break;
                } else if (line[x + 1] == '*') {
                    size_t oY = y; // original y 
                    size_t pos = string::npos;
                    for (; y < lines.size(); ++y) {
                        pos = lines[y].find("*/");
                        if (pos != string::npos) {
                            for (; oY < y; ++oY) {
                                for (; x < lines[oY].size(); ++x) {
                                    w.writeYellow(lines[oY][x], curY, curX++);
                                    checkBounds(curY, curX);
                                }
                                x = 0;
                                curX = 0;
                                ++curY;
                            }
                            for (; x <= pos + 1; ++x) {
                                w.writeYellow(lines[y][x], curY, curX++);
                                checkBounds(curY, curX);
                            }
                            break;
                        }
                    }
                    // always entering the loop
                    if (pos == string::npos) {
                        for (; oY < lines.size(); ++oY) {
                            for (; x < lines[oY].size(); ++x) {
                                w.writeChar(lines[oY][x], curY, curX++);
                                checkBounds(curY, curX);
                            }
                            x = 0;
                        }
                        return;
                    }
                } else {
                    w.writeChar(c, curY, curX++);
                    checkBounds(curY, curX);
                    ++x;
                }
                
            } else if (c == '#' && (line[x + 1] == ' ' || line[x + 1] == '\t')) {
                // preprocessor
                // print entire line in colour
                for (; x < line.size(); ++x) {
                    w.writeTurq(line[x], curY, curX++);
                    checkBounds(curY, curX);
                }
            } else {
                // garbage, can print
                w.writeChar(c, curY, curX++);
                checkBounds(curY, curX);
                w.refresh();
                ++x;
            }
        }
        ++y;
        x = 0;
        ++curY;
        curX = 0;
    }
    
    for (; y <= maxY; ++y) w.writeFill(y);
}

void Syntax::checkBounds(size_t &curY, size_t &curX) {
    if (static_cast<int>(curX) == t.getWidth()) {
        curX = 0;
        ++curY;
    }
}
}
