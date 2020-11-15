#include <fstream>
#include <iostream>
#include <string>

#include "models/text-display.h"

using namespace std;
using namespace vm;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: vm [file]" << std::endl;
        return 2;
    }
    models::TextDisplay textModel{argc == 2 ? argv[1] : ""};
    textModel.run();
    return 0;
}