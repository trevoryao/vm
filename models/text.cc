#include "text.h"

#include <fstream>
#include <vector>
#include <string>

#include "file.h"

using namespace std;

namespace models {
Text::Text(const string &fileName) : topLine{0}, botLine{0} {
    if (fileName != "") {
        fstream fs{fileName};
        File file{fs};
        text.push_back(file.read());
    }
}

Text::Text(const string &fileName, int maxX) : topLine{0} {
    string data;

    if (fileName != "") {
        fstream fs{fileName};
        File file{fs};
        data = file.read();
    }

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
    botLine = maxX > static_cast<int>(text.size()) ? text.size() - 1 : maxX - 1;
}

const vector<string> &Text::getText() { return text; }

void Text::resizeText(int maxX) {
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
}
}
