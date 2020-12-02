#include "keyboard.h"

#include "action.h"

namespace actions {
Keyboard::Keyboard(KeyType value, char key) : IAction{ActionType::KEYBOARD, value}, key{key} { }

char Keyboard::getKey() { return key; }
}
