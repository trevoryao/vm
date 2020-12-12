#ifndef __ROW_H__
#define __ROW_H__

#define INDENT_SIZE 4

#include <iostream>
#include <string>
#include <vector>

namespace models {
class Row final {
    friend class Text;
private:
    int width;
    std::vector<std::string> rows;
public:
    explicit Row(int maxX);
    Row(const std::string &s, int maxX);
    
    const std::vector<std::string> &getRows() const;
    
    void resize(int maxX);
    
    const char &operator[](size_t i) const;
    char &operator[](size_t i);
    
    Row &operator+(const Row &other);
    
    size_t getHeight() const;
    size_t size() const;
    
    void getPos(int &y, int &x);
    
    void insert(int x, char c);
    void insert(int x, const std::string &s);
    void insert(int x, const Row &row);
    
    void append(char c);
    void indent(int x);
    
    void popBack();
    
    Row subRow(size_t pos) const;
    Row subRow(size_t pos, size_t len) const;

    char erase(size_t pos);
    
    void clear(); // "\n"
    
    size_t find(const std::string &s, size_t pos) const;
    size_t rfind(const std::string &s, size_t pos) const;
    
    std::iostream &out(std::iostream &out) const;
private:
    std::string toString() const;
};

std::iostream &operator<<(std::iostream &out, const Row &row);
}

#endif
