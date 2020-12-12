#ifndef __TEXT_H__
#define __TEXT_H__

#include <vector>
#include <string>

#include "file.h"
#include "row.h"

namespace models {
class Text final {
private:
    int topLine, botLine, width;
    bool cpp;
    File file;
    std::vector<Row> text;
public:
    Text(const std::string &fileName, int maxY, int maxX);
    
    const std::string &getFileName();
    void setFileName(const std::string &fileName);
    
    const std::vector<Row> &getTextFile();
    
    bool hasFile();
    
    bool isCpp();
    
    int getTopLine();
    int getBotLine();
    void setBotLine(int n);
    
    int getWidth();
    
    int getLines();
    int getChars();

    void getWindowCursor(int &y, int &x);
    
    size_t height();
    
    void resizeText(int maxY, int maxX);
    
    void write();
    // true if files are different, false otherwise
    bool diff();
    
    bool insertFile(const std::string &filePath, int y, int &height);
    
    void insert(char c, int y, int x); // insert before
    void insert(const Row &row, int y, int x);
    void insert(Row &row, int y);
    
    char backSpace(int y, int x);
    char del(int y, int x);
    
    char delChar(int y, int x); // at
    Row delLine(int y);
    
    Row changeLine(int y);
    void replaceLine(const Row &row, int y);
    
    char replaceChar(char c, int y, int x);
    
    void newLine(int y, int x);
    void newLine(int y);
    
    void indent(int y, int x);
    
    void scrollUp(int n);
    void scrollDown(int n);

    std::vector<std::string> getFullLines();
    std::string getPreText();
};
}

#endif
