#include "file.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <sys/wait.h>

using namespace std;

namespace models {
File::File(const string &fileName): fileName{fileName} { }

const std::string &File::getName() { return fileName; }

string File::read() {
    fstream f{fileName};
    if (f.fail()) throw FileDNE{};
    string file;
    f.seekg(0, ios::end);   
    file.resize(f.tellg());
    f.seekg(0, ios::beg);
    f.read(&file[0], file.size());
    return file;
}

void File::write(const vector<string> &tmpFile) {
    fstream f{fileName};
    for (auto &line : tmpFile) f << line;
}

bool File::diff(const vector<string> &tmpFile) {
    string tempDir;
    
    FILE *temp = popen("mktemp", "r");
    for (char c = fgetc(temp); c != EOF; c = fgetc(temp)) {
        if (c != '\n') tempDir.push_back(c);
    }
    pclose(temp);
    
    fstream f{tempDir};
    for (auto &line : tmpFile) f << line << endl;
    f.close();
    
    bool isDiff = WEXITSTATUS(system(("diff " + fileName + " " + tempDir + " > /dev/null").c_str())) == 1;
    system(("rm " + tempDir + " > /dev/null").c_str());
    return isDiff;
}
}
