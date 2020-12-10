#include "file.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <sys/wait.h>

#include "row.h"
#include "../exceptions/file-dne.h"

using namespace exceptions;
using namespace std;

namespace models {
File::File(const string &fileName) : fileName{fileName} { }

const string &File::getName() { return fileName; }

void File::setName(const string &newName) {
    fstream f{fileName};
    if (f.fail()) {
        f.close();
        throw FileDNE{};
    }
    fileName = newName;
}

vector<string> File::read() {
    fstream f{fileName};
    if (f.fail()) {
        f.close();
        throw FileDNE{};
    }
    vector<string> file;
    string line;
    while (getline(f, line)) {
        line.push_back('\n');
        file.push_back(line);
    }
    return file;
}

void File::write(const vector<Row> &tmpFile) {
    fstream f{fileName, ios::trunc | ios::out};
    for (auto &line : tmpFile) f << line;
    f.close();
}

bool File::diff(const vector<Row> &tmpFile) {
    string tempDir;
    
    FILE *temp = popen("mktemp", "r");
    for (char c = fgetc(temp); c != EOF; c = fgetc(temp)) {
        if (c != '\n') tempDir.push_back(c);
    }
    pclose(temp);
    
    fstream f{tempDir};
    for (auto &line : tmpFile) f << line;
    f.close();
    
    int cmdStatus = system(("diff " + fileName + " " + tempDir + " > /dev/null").c_str());
    bool isDiff = WEXITSTATUS(cmdStatus) == 1;
    system(("rm " + tempDir + " > /dev/null").c_str());
    return isDiff;
}
}
