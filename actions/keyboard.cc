#include "keyboard.h"

namespace actions {
Keyboard::Keyboard(KeyType value, char key) : IAction{value}, key{key} { }

char Keyboard::getKey() { return key; }
}
