#include "text.h"

#include <fstream>
#include <vector>
#include <string>

#include "file.h"

using namespace std;

namespace models {
Text::Text(const string &fileName, int maxY, int maxX) : topLine{0}, width{maxX}, file{fileName} {
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

const vector<Row> &Text::getTextFile() { return text; }

bool Text::hasFile() { return getFileName().size() != 0; }

int Text::getTopLine() { return topLine; }
int Text::getBotLine() { return botLine; }
void Text::setBotLine(int n) { botLine = n; }

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

bool Text::insert(const std::string &filePath, int y, int &height) {
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

void Text::insertAt(const std::string &s, int y, int x) {
    text[y].insert(x, s);
}

// only removes, thats it
void Text::delChar(int y, int x) { // check beginning of line
    text[y].erase(x);
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

void Text::changeLine(int y) {
    text[y].clear();
}

void Text::delLine(int y) {
    text.erase(text.begin() + y);
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
}
