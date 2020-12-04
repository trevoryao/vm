#include <fstream>
#include <iostream>
#include <string>

#include <ncurses.h>

#include "models/text-model.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: vm [file]" << std::endl;
        return 2;
    }

    models::TextModel textModel{argc == 2 ? argv[1] : ""};
    textModel.run();
    
    return 0;
}
