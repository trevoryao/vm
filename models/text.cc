#include "text.h"

#include <fstream>
#include <vector>
#include <string>

#include "file.h"
#include "../exceptions/file-dne.h"

using namespace exceptions;
using namespace std;

namespace {
inline bool isOpenB(char c) {
    return c == '(' || c == '{' || c == '[';
}

inline bool isCloseB(char c) {
    return c == ')' || c == '}' || c == ']';
}

inline bool isMatch(char c, char o) {
    return (o == '(' && c == ')') || (o == '{' && c == '}') 
        || (o == '[' && c == ']');
}
}

namespace models {
Text::Text(const string &fileName, int maxY, int maxX) : topLine{0}, width{maxX}, 
    cpp{(fileName.size() > 2 && fileName.back() == 'h') ||
    (fileName.size() > 3 && fileName.substr(fileName.size() - 2) == "cc")},
    file{fileName} {
    try {
        vector<string> data = file.read();
        
        for (auto &line : data) {
            text.push_back(Row{line, maxX});
        }

        botLine = static_cast<size_t>(maxY) > text.size() ? text.size() - 1 : maxY - 1;
    } catch (FileDNE &e) {
        text.push_back(Row{maxX});
        botLine = 0;
    }
}

const string &Text::getFileName() { return file.getName(); }

void Text::setFileName(const string &fileName) { 
    file.setName(fileName);
    cpp = (fileName.size() > 2 && fileName.back() == 'h') ||
        (fileName.size() > 3 && fileName.substr(fileName.size() - 2) == "cc");
}

const vector<Row> &Text::getTextFile() { return text; }

bool Text::hasFile() { return getFileName().size() != 0; }

bool Text::isCpp() { return cpp; }

int Text::getTopLine() { return topLine; }
int Text::getBotLine() { return botLine; }
void Text::setBotLine(int n) { botLine = n; }

int Text::getWidth() { return width; }

int Text::getLines() { return text.size(); }
int Text::getChars() {
    size_t size = 0;
    for (auto &row : text) size += row.size();
    return size;
}

void Text::getWindowCursor(int &y, int &x) {
    int height = 0;
    for (int i = 0; i < y; ++i) {
        height += text[i].getHeight();
    }
    int height2 = 0, width2 = x;
    text[y].getPos(height2, width2);
    y = height + height2;
    x = width2;
}

void Text::resizeText(int maxY, int maxX) {
    for (auto &row : text) {
        row.resize(maxX);
    }
    botLine = static_cast<size_t>(maxY) > height() ? height() - 1 : maxY - 1;
    width = maxX;
}

void Text::write() { file.write(text); }

bool Text::diff() { return file.diff(text); }

bool Text::insertFile(const std::string &filePath, int y, int &height) {
    try {
        vector<string> data = file.read();
        
        int j = 1;
        for (auto &line : data) {
            text.insert(text.begin() + y + j++, Row{line, width});
        }

    height = j - 1;
    } catch (FileDNE &e) {
        return false;
    }
    return true;
}

void Text::insert(char c, int y, int x) {
    text[y].insert(x, c);
}

void Text::insert(const Row &row, int y, int x) {
    text[y].insert(x, row);
}

void Text::insert(Row &row, int y) {
    text.insert(text.begin() + y + 1, row);
}

// only removes, thats it
char Text::delChar(int y, int x) { // check beginning of line
    char c = text[y][x];
    text[y].erase(x);
    return c;
}

char Text::backSpace(int y, int x) {
    if (y == 0 && x == 0) return -1;
    
    if (x != 0) {
        return text[y].erase(x - 1);
    } else {
        text[y - 1].popBack();
        text[y - 1] = text[y - 1] + text[y];
        text.erase(text.begin() + y);
        return '\n';
    }
}

char Text::del(int y, int x) {
    if (static_cast<size_t>(y) == text.size() - 1 && 
        static_cast<size_t>(x) == text[y].size() - 1) return -1;
    
    if (static_cast<size_t>(x) != text[y].size() - 1) {
        return text[y].erase(x);
    } else {
        text[y].popBack();
        text[y] = text[y] + text[y + 1];
        text.erase(text.begin() + y + 1);
        return '\n';
    }
}

Row Text::changeLine(int y) {
    Row r = text[y];
    text[y].clear();
    return r;
}

void Text::replaceLine(const Row &row, int y) {
    text[y] = row;
}

char Text::replaceChar(char c, int y, int x) {
    char orig = text[y][x];
    text[y][x] = c;
    return orig;
}

Row Text::delLine(int y) {
    Row r = *(text.begin() + y);
    text.erase(text.begin() + y);
    return r;
}

void Text::newLine(int y, int x) {
    text.insert(text.begin() + y + 1, text[y].subRow(x));
    auto row = text[y].subRow(0, x);
    row.append('\n');
    text[y] = row;
}

void Text::newLine(int y) {
    text.insert(text.begin() + y + 1, Row{width});
}

void Text::indent(int y, int x) {
    text[y].indent();
}

void Text::scrollUp(int lines) {
    int newTop = topLine - lines;
    int newBottom = botLine - lines;
    if (newTop < 0) {
        newBottom -= newTop;
        newTop = 0;
    }
    
    topLine = newTop;
    botLine = newBottom;
}

void Text::scrollDown(int lines) {
    int newTop = topLine + lines;
    int newBottom = botLine + lines;
    
    if (static_cast<size_t>(newBottom) >= text.size()) {
        int diff = text.size() - newBottom - 1;
        newTop += diff;
        newBottom += diff;
    }
    
    topLine = newTop;
    botLine = newBottom;
}

size_t Text::height() {
    size_t size = 0;
    for (auto &row : text) size += row.getHeight();
    return size;
}

vector<string> Text::getFullLines() {
    vector<string> lines;
    lines.reserve(botLine - topLine);
    
    string l;
    
    for (int i = topLine; i <= botLine; ++i) {
        lines.push_back(text[i].toString());
    }
    
    return lines;
}

string Text::getPreText() {
    string preText;
    for (int i = 0; i <= botLine; ++i) {
        preText.append(text[i].toString());
    }
    return preText;
}

char Text::getMatchingBracket(int &y, int &x) {
    vector<char> s{text[y][x]};
    if (isOpenB(text[y][x])) {
        ++x;
        while (static_cast<size_t>(y) < text.size()) {
            while (static_cast<size_t>(x) < text[y].size()) {
                if (isOpenB(text[y][x])) {
                    s.push_back(text[y][x]);
                } else if (isCloseB(text[y][x])) {
                    if (isMatch(text[y][x], s.back())) {
                        s.pop_back();
                        if (s.empty()) return text[y][x];
                    }
                }
                ++x;
            }
            x = 0;
            ++y;
        }
    } else if (isCloseB(text[y][x])) {
        --x;
        while (y >= 0) {
            while (x >= 0) {
                if (isOpenB(text[y][x])) {
                    if (isMatch(s.back(), text[y][x])) {
                        s.pop_back();
                        if (s.empty()) return text[y][x];
                    }
                } else if (isCloseB(text[y][x])) {
                    s.push_back(text[y][x]);
                }
                --x;
            }
            x = text[--y].size();
        }
    }
    return 0;
}
}
