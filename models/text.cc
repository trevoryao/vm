#include "text.h"

#include <fstream>
#include <vector>
#include <string>

#include "file.h"

using namespace std;

namespace models {
Text::Text(const string &fileName) : topLine{0}, botLine{0}, file{fileName} {
    if (fileName.size()) text.push_back(file.read());
}

Text::Text(const string &fileName, int maxY, int maxX) : topLine{0}, file{fileName} {
    string data;

    if (fileName.size()) data = file.read();
    
    string line;
    line.reserve(maxX);
    int curWidth = 0;
    for (size_t i = 0; i < data.size(); ++i, ++curWidth) {
        if (curWidth == maxX) {
            text.push_back(line);
            line.clear();
            curWidth = 0;
        }
        
        line.push_back(data[i]);
        
        if (data[i] == '\n') {
            text.push_back(line);
            line.clear();
            curWidth = 0;
        }
    }
    botLine = static_cast<size_t>(maxY) > text.size() ? text.size() - 1 : maxY - 1;
}

const string &Text::getFileName() { return file.getName(); }

const vector<string> &Text::getTextFile() { return text; }

int Text::getTopLine() { return topLine; }
int Text::getBotLine() { return botLine; }

void Text::resizeText(int maxY, int maxX) {
    vector<string> newText;
    
    int curX = 0;
    string line;
    line.reserve(maxX);
    for (auto &v : text) {
        for (auto c : v) {
            if (curX == maxX) {
                newText.push_back(line);
                line.clear();
                curX = 0;
            }
            
            line.push_back(c);
            ++curX;
            if (c == '\n') {
                newText.push_back(line);
                line.clear();
                curX = 0;
            }
            
        }
    }
    
    text = newText;
    botLine = static_cast<size_t>(maxY) > text.size() ? text.size() - 1 : maxY - 1;
}

void Text::write() { file.write(text); }

bool Text::diff() { return file.diff(text); }

bool Text::insert(const std::string &filePath, int y, int maxX) {
    try {
        File newFile{filePath};
        string data = newFile.read();
        
        string line;
        int curWidth = 0;
        auto it = text.begin();
        it += y + 1;
        
        for (size_t i = 0; i < data.size(); ++i, ++curWidth) {
            if (curWidth == maxX) {
                text.insert(it, line);
                line.clear();
                curWidth = 0;
            }
            
            line.push_back(data[i]);
            
            if (data[i] == '\n') {
                text.insert(it, line);
                line.clear();
                curWidth = 0;
            }
        }
    } catch (FileDNE &e) {
        return false;
    }
    return true;
}

void Text::insert(char c, int y, int x) {
    text[y].insert(x, 1, c);
}

void Text::delChar(int y, int x) { // check beginning of line
    if (x != 0) {
        text[y].erase(x, 1);
    } else {
        text[y - 1].pop_back();
        text[y - 1] = text[y - 1] + text[y];
        auto it = text.begin() + y + 1;
        text.erase(it);
    }
}

void Text::changeLine(int y) {
    text[y] = "\n";
}

void Text::delLine(int y) {
    text.erase(text.begin() + y);
}

void Text::newLine(int y, int x) {
    // char at pos goes
    auto it = text.begin() + y + 1;
    
    if (text[y][x] == '\n') {
        // end of line
        text.insert(it, "\n");
    } else {
        text.insert(it, text[y].substr(text[y].size() - x));
        text[y] = text[y].substr(0, x) + "\n";
    }
}

void Text::newLine(int y) {
    text.insert(text.begin() + y + 1, "\n");
}

void Text::indent(int y, int x) {
    text[y] = "    " + text[y];
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
}
