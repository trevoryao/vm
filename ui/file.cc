#include "file.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace vm::ui {
File::File(fstream &f): f{f} { }

vector<string> &File::read() {
    vector<string> tmpFile;
    string line;
    while (getline(f, line)) tmpFile.push_back(line);
    return tmpFile;
}

void File::write(const vector<string> &tmpFile) {
    for (auto &line : tmpFile) f << line;
}
}
