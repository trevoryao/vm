#include "key-input.h"

#include <exception>
#include <ncurses.h>
#include <map>
#include <memory>

#include "../actions/action.h"
#include "../actions/keyboard.h"

using namespace actions;
using namespace std;

namespace controllers {
KeyInput::KeyInput() :
    actionMap{
        {KEY_BACKSPACE, ActionType::KEY},
        {'\n', ActionType::KEY},
        {13, ActionType::KEY},
        // {KEY_DC, ActionType::KEY},
        {'\t', ActionType::KEY},
        {27, ActionType::KEY},
        {KEY_LEFT, ActionType::MVT},
        {KEY_RIGHT, ActionType::MVT},
        {KEY_UP, ActionType::MVT},
        {KEY_DOWN, ActionType::MVT}
    },
    keyMap{
        {KEY_BACKSPACE, KeyType::BACKSPACE},
        {'\n', KeyType::RETURN},
        {13, KeyType::RETURN},
        // {KEY_DC, KeyType::DEL},
        {'\t', KeyType::TAB},
        {27, KeyType::ESC}
    },
    mvtMap{
        {KEY_LEFT, MvtType::LEFT},
        {KEY_RIGHT, MvtType::RIGHT},
        {KEY_UP, MvtType::UP},
        {KEY_DOWN, MvtType::DOWN}
    } { }

unique_ptr<Action> KeyInput::action(Incomplete *) { return unique_ptr<Action>{}; }

unique_ptr<Action> KeyInput::action() {
    int c = getch();
    
    if (' ' <= c && c <= '~') {
        return make_unique<Keyboard>(KeyType::ALPHA_NUM, c);
    }
    /*
    try {
        return make_unique<Keyboard>(keyMap.at(c));
    } catch (out_of_range &e) {
        return unique_ptr<Keyboard>{};
    }*/
    try {
        switch (actionMap.at(c)) {
            case ActionType::KEY: return make_unique<Keyboard>(keyMap.at(c));
            case ActionType::MVT: return make_unique<Movement>(mvtMap.at(c));
        }
    } catch (out_of_range &e) {
        return unique_ptr<Keyboard>{};
    }
    return unique_ptr<Keyboard>{};
}
}
