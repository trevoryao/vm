#ifndef __TEXT_H__
#define __TEXT_H__

#include <vector>
#include <string>

#include "file.h"

namespace models {
class Text final {
private:
    int topLine, botLine, width;
    File file;
    std::vector<std::string> text;
public:
    Text(const std::string &fileName, int maxY, int maxX);
    
    const std::string &getFileName();
    const std::vector<std::string> &getTextFile();
    
    bool hasFile();
    
    int getTopLine();
    int getBotLine();
    void setBotLine(int n);
    
    int getLines();
    int getChars();
    
    void resizeText(int maxY, int maxX);
    
    void write();
    // true if files are different, false otherwise
    bool diff();
    
    bool insert(const std::string &filePath, int y, int &height);
    
    void insert(char c, int y, int x); // insert before
    void backSpace(int y, int x);
    void del(int y, int x);
    
    void delChar(int y, int x); // at
    void delLine(int y);
    void changeLine(int y);
    void newLine(int y, int x);
    void newLine(int y);
    
    void indent(int y, int x);
    
    void scrollUp(int n);
    void scrollDown(int n);
};
}

#endif
