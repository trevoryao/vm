#include <iostream>

#include "models/text-model.h"
#include "ui/graphics.h"

using namespace std;
// this is the first real edit using vi
int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: vm [file]" << std::endl;
        return 2;
    }
    
    try {
        ui::Graphics graphics;
        models::TextModel textModel{argc == 2 ? argv[1] : ""};
        textModel.run();
    } catch (...) { }
    
    return 0;
}
