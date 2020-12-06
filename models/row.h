#ifndef __ROW_H__
#define __ROW_H__

#include <iostream>
#include <string>
#include <vector>

namespace models {
class Row final {
private:
    int width;
    std::vector<std::string> rows;
public:
    // "\n"
    explicit Row(int maxX);
    Row(const std::string &s, int maxX);
    
    const std::vector<std::string> &getRows() const;
    
    void resize(int maxX);
    
    const char &operator[](size_t i) const;
    
    Row &operator+(const Row &other);
    
    size_t getHeight() const;
    size_t size() const;
    
    void getPos(int &y, int &x);
    
    void insert(int x, char c);
    
    void append(char c);
    void indent();
    
    void popBack();
    
    Row subRow(size_t pos);
    Row subRow(size_t pos, size_t len);

    void erase(size_t pos);
    
    void clear(); // "\n"
    
    std::iostream &out(std::iostream &out) const;
private:
    std::string toString() const;
};

std::iostream &operator<<(std::iostream &out, const Row &row);
}

#endif
