#include "row.h"

#define INDENT_SIZE 4

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace models {
Row::Row(int maxX) : width{maxX}, rows{"\n"} { }

Row::Row(const std::string &s, int maxX) : width{maxX} {
    for (size_t i = 0; i < s.size(); i += maxX) rows.push_back(s.substr(i, maxX));
}

void Row::resize(int maxX) {
    std::string s = toString();
    rows.clear();
    for (size_t i = 0; i < s.size(); i += maxX) rows.push_back(s.substr(i, maxX));
    width = maxX;
}

const std::vector<std::string> &Row::getRows() const { return rows; }

const char &Row::operator[](size_t i) const {
    return rows[i / width][i % width];
    /*
    for (auto &row : rows) {
        if (i < row.size()) return row[i];
        else i -= row.size();
    }*/
}

Row &Row::operator+(const Row &other) {
    // always go off own width
    std::string s = other.toString();
    rows[rows.size() - 1] += s.substr(0, width - rows[rows.size() - 1].size());
    //s = s.substr(width - rows[rows.size() - 1].size());
    for (size_t i = width - rows[rows.size() - 1].size(); i < s.size(); i += width) {
        rows.push_back(s.substr(i, width));
    }
    
    return *this;
}

std::string Row::toString() const {
    std::stringstream ss;
    for (auto row : rows) ss << row;
    return ss.str();
}

size_t Row::getHeight() const { return rows.size(); }

size_t Row::size() const { 
    size_t size = 0;
    for (auto &row : rows) size += row.size();
    return size;
}

void Row::getPos(int &y, int &x) {
    y = x / width;
    x = x % width;
}

void Row::insert(int x, char c) {
    rows[x / width].insert(x % width, 1, c);
}

void Row::append(char c) {
    if (rows[rows.size() - 1].size() == static_cast<size_t>(width)) {
        rows.push_back(std::string(1, c));
    } else {
        rows[rows.size() - 1].push_back(c);
    }
}

void Row::indent() {
    rows[0] = std::string(INDENT_SIZE, ' ') + rows[0];
    std::string overflow;
    for (auto &row : rows) {
        row = overflow + row;
        overflow = row.substr(width);
        row = row.substr(0, width);
    }
}

void Row::popBack() { rows[rows.size() - 1].pop_back(); }

Row Row::subRow(size_t pos) {
    return Row{toString().substr(pos), width};
}

Row Row::subRow(size_t pos, size_t len) {
    return Row{toString().substr(pos, len), width};
}

void Row::erase(size_t pos) {
    size_t i = pos / width;
    rows[i].erase(pos % width, 1);
    for (; i + 1 < rows.size(); ++i) {
        rows[i].push_back(rows[i + 1][0]);
        rows[i + 1] = rows[i + 1].substr(1);
    }
}

void Row::clear() {
    rows.clear();
    rows.push_back("\n");
}

std::iostream &Row::out(std::iostream &out) const {
    for (auto &row : rows) out << row;
    return out;
}

std::iostream &operator<<(std::iostream &out, const Row &row) {
    return row.out(out);
}
}