#include "file.h"

#include <fstream>
#include <streambuf>
#include <string>
#include <vector>

using namespace std;

namespace models {
File::File(fstream &f): f{f} { }

string File::read() {
    string file;
    f.seekg(0, ios::end);   
    file.resize(f.tellg());
    f.seekg(0, ios::beg);
    f.read(&file[0], file.size());
    return file;
}

void File::write(const vector<string> &tmpFile) {
    for (auto &line : tmpFile) f << line;
}
}
