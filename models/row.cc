#include "row.h"

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
}

char &Row::operator[](size_t i) {
    return rows[i / width][i % width];
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

bool Row::isBlank() const {
    return rows.size() == 1 && rows[0] == "\n";
}

void Row::getPos(int &y, int &x) {
    y = x / width;
    x = x % width;
}

void Row::insert(int x, char c) {
    rows[x / width].insert(x % width, 1, c);
}

void Row::insert(int x, const std::string &s) {
    rows[x / width].insert(x % width, s);
}

void Row::insert(int x, const Row &row) {
    return insert(x, row.toString());
}

void Row::append(char c) {
    if (rows.size() == 0) rows.push_back(std::string(1, c));
    else if (rows.back().size() == static_cast<size_t>(width)) {
        rows.push_back(std::string(1, c));
    } else {
        rows.back().push_back(c);
    }
}

void Row::indent(int x) {
    insert(x, std::string(INDENT_SIZE, ' '));
}

void Row::popBack() { rows[rows.size() - 1].pop_back(); }

Row Row::subRow(size_t pos) const {
    return Row{toString().substr(pos), width};
}

Row Row::subRow(size_t pos, size_t len) const {
    return Row{toString().substr(pos, len), width};
}

char Row::erase(size_t pos) {
    size_t i = pos / width;
    char erased = rows[i][pos % width];
    rows[i].erase(pos % width, 1);
    for (; i + 1 < rows.size(); ++i) {
        if (rows[i + 1].size() == 1) {
            rows.pop_back();
            return erased;
        }
        rows[i].push_back(rows[i + 1][0]);
        rows[i + 1] = rows[i + 1].substr(1);
    }
    return erased;
}

void Row::clear() {
    rows.clear();
    rows.push_back("\n");
}

size_t Row::find(const std::string &s, size_t pos) const {
    return toString().find(s, pos);
}

size_t Row::rfind(const std::string &s, size_t pos) const {
    return toString().rfind(s, pos);
}

std::iostream &Row::out(std::iostream &out) const {
    for (auto &row : rows) out << row;
    return out;
}

std::iostream &operator<<(std::iostream &out, const Row &row) {
    return row.out(out);
}
}
