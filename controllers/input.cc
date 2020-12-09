#include "input.h"

#include <exception>
#include <memory>
#include <ncurses.h>
#include <string>

#include "../actions/action.h"
#include "../actions/e-movement.h"
#include "../actions/e-search.h"
#include "../actions/file-op.h"
#include "../actions/global.h"
#include "../actions/insert.h"
#include "../actions/movement.h"
#include "../actions/replace.h"
#include "../actions/scroll.h"
#include "../actions/search.h"
#include "../actions/text-edit.h"
#include "../exceptions/bad-entry.h"
#include "../exceptions/clear-cmd.h"
#include "../exceptions/update-cmd.h"

using namespace actions;
using namespace exceptions;
using namespace std;

namespace {
inline bool isDisplayChar(int c) {
    return 32 <= c && c <= 126;
}
}

namespace controllers {
Input::Input() :
    actionCharMap{
        {'h', ActionType::MVT},
        {'l', ActionType::MVT},
        {'k', ActionType::MVT},
        {'j', ActionType::MVT},
        {'b', ActionType::MVT},
        {'w', ActionType::MVT},
        {'0', ActionType::MVT},
        {'^', ActionType::MVT},
        {'$', ActionType::MVT},
        {KEY_LEFT, ActionType::MVT},
        {KEY_RIGHT, ActionType::MVT},
        {KEY_DOWN, ActionType::MVT},
        {KEY_UP, ActionType::MVT},
        {'i', ActionType::INS},
        {'a', ActionType::INS},
        {'O', ActionType::INS},
        {'o', ActionType::INS},
        {'I', ActionType::INS},
        {'A', ActionType::INS},
        // {'c', ActionType::INS},
        {'S', ActionType::INS},
        {'s', ActionType::INS},
        {'r', ActionType::REPLACE},
        {'R', ActionType::REPLACE},
        {'r', ActionType::REPLACE},
        // {'d', ActionType::TEXT_EDIT},
        {'x', ActionType::TEXT_EDIT},
        {'X', ActionType::TEXT_EDIT},
        {'p', ActionType::TEXT_EDIT},
        {'P', ActionType::TEXT_EDIT},
        {'u', ActionType::TEXT_EDIT},
        {'.', ActionType::TEXT_EDIT},
        // {'y', ActionType::TEXT_EDIT},
        {'J', ActionType::TEXT_EDIT},
        {'f', ActionType::SEARCH},
        {'F', ActionType::SEARCH},
        {';', ActionType::SEARCH},
        {'n', ActionType::SEARCH},
        {'N', ActionType::SEARCH}
    },
    mvtMap{
        {'h', MvtType::LEFT},
        {'l', MvtType::RIGHT},
        {'k', MvtType::UP},
        {'j', MvtType::DOWN},
        {KEY_LEFT, MvtType::LEFT},
        {KEY_RIGHT, MvtType::RIGHT},
        {KEY_UP, MvtType::UP},
        {KEY_DOWN, MvtType::DOWN},
        {'b', MvtType::WORD_LEFT},
        {'w', MvtType::WORD_RIGHT},
        {'0', MvtType::BEG_LINE},
        {'^', MvtType::BEG_CH},
        {'$', MvtType::END_CH}
    },
    insMap{
        {'i', InsType::BEFORE},
        {'a', InsType::AFTER},
        {'O', InsType::NEW_LINE_ABOVE},
        {'o', InsType::NEW_LINE_BELOW},
        {'I', InsType::BEG_LINE}, // before char
        {'A', InsType::END_LINE},
        // {"c", InsType::CH_MVT},
        // {"cc", InsType::CH_LINE},
        {'S', InsType::CH_LINE},
        {'s', InsType::CH}
    },
    replaceMap{
        {'r', ReplaceType::RPL_UNDER},
        {'R', ReplaceType::RPL}
    },
    fileOpMap{
        {":w", FileOpType::WRITE},
        {":wq", FileOpType::WRITE_QUIT},
        {":q", FileOpType::QUIT},
        {":q!", FileOpType::FORCE_QUIT},
        {":r", FileOpType::INSERT}
    },
    eMvtMap{
        {":0", EMvtType::TOP},
        {":$", EMvtType::BOTTOM}
        // {":", EMvtType::LINE_NUM} integer matching
    },
    textEditMap{
        // {"dd", TextEditType::DEL_LINE},
        // {"d", TextEditType::DEL_MVT},
        {'x', TextEditType::DEL_UNDER},
        {'X', TextEditType::DEL_BEFORE},
        {'p', TextEditType::PUT_AFTER},
        {'P', TextEditType::PUT_BEFORE},
        {'u', TextEditType::UNDO},
        {'.', TextEditType::REPEAT},
        // {"yy", TextEditType::YANK_LINE},
        // {"y", TextEditType::YANK_MVT},
        {'J', TextEditType::JOIN}
    },
    searchMap{
        {'f', SearchType::NEXT_CHAR},
        {'F', SearchType::PREV_CHAR},
        {';', SearchType::REPEAT_CHAR},
        {'n', SearchType::FWD_SEARCH},
        {'N', SearchType::PREV_SEARCH}
    },
    eSearchMap{
        {'/', ESearchType::NEW_FWD_SEARCH},
        {'?', ESearchType::NEW_PREV_SEARCH}
    },
    scrollMap{
        {'F', ScrollType::PG_FWD},
        {'B', ScrollType::PG_BACK},
        {'D', ScrollType::LINE_FWD},
        {'U', ScrollType::LINE_BACK}
    } { }

unique_ptr<Action> Input::action() {
    int c = getch();

    switch (c) {
        case ERR: return unique_ptr<Action>{};
        case KEY_RESIZE: return make_unique<Global>(GlobalType::RESIZE);
        case ':': return make_unique<Incomplete>(IncType::EXEC, c);
        case '/': return make_unique<ESearch>(ESearchType::NEW_FWD_SEARCH);
        case '?': return make_unique<ESearch>(ESearchType::NEW_PREV_SEARCH);
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return make_unique<Incomplete>(IncType::UNKNOWN, static_cast<char>(c));
        case 'c':
        case 'd':
        case 'y':
            return make_unique<Incomplete>(IncType::STATIC, static_cast<char>(c));
        default: return parseAction(c, 1);
    }
}

unique_ptr<Action> Input::parseAction(int c, int n) { // TODO: multiplier
    try {
        ActionType type = actionCharMap.at(c);
        
        switch (type) {
            case ActionType::MVT: return make_unique<Movement>(mvtMap.at(c), n);
            case ActionType::INS: return make_unique<Insert>(insMap.at(c), n);
            case ActionType::REPLACE: return make_unique<Replace>(replaceMap.at(c), n);
            case ActionType::TEXT_EDIT: return make_unique<TextEdit>(textEditMap.at(c), n);
            // get search later
            case ActionType::SEARCH: return make_unique<Search>(searchMap.at(c), n);
            default: return unique_ptr<Action>{};
        }
    } catch (out_of_range &e) {
        string ctrl = unctrl(c);
        if (ctrl.size() != 2) return unique_ptr<Action>{};
        try {
            if (ctrl[1] == 'G') return make_unique<Global>(GlobalType::DISPLAY_FILE);
            return make_unique<Scroll>(scrollMap.at(ctrl[1]));
        } catch (out_of_range &e) {
            return unique_ptr<Action>{};
        }
    }
}

unique_ptr<Action> Input::action(Incomplete *a) {
    int c = getch();
    
    if (c == ERR) {
        return unique_ptr<Action>{};
    } else if (c == KEY_RESIZE) return make_unique<Global>(GlobalType::RESIZE);
    
    // if esc, should exit (throw parse error
    if (c == 27) throw ClearCmd{};
    
    switch (a->getValue()) {
        case IncType::UNKNOWN: {
            if (c == KEY_BACKSPACE) throw ClearCmd{};
            if ('0' <= c && c <= '9') {
                a->addFragment(static_cast<char>(c));
                throw UpdateCmd{};
            } else if (c == 'c' || c == 'd' || c == 'y') {
                return make_unique<Incomplete>(IncType::STATIC, 
                    static_cast<char>(c), stoi(a->getFragment()));
            } else {
                return parseAction(c, stoi(a->getFragment()));
            }
        }
        case IncType::EXEC: {
            if (c == 10 || c == 13) {
                if (a->getFragment().size() > 1) {
                    if (a->getFragment() == ":$") {
                        return make_unique<EMovement>(EMvtType::BOTTOM);
                    } else if (a->getFragment()[1] == 'r') {
                        if (a->getFragment()[2] != ' ') throw BadEntry{a->getFragment()};
                        return make_unique<FileOp>(FileOpType::INSERT, a->getFragment().substr(3));
                    }
                    try {
                        int lineNum = stoi(a->getFragment().substr(1, a->getFragment().size() - 1));
                        if (lineNum == 0) {
                            return make_unique<EMovement>(EMvtType::TOP);
                        }
                        return make_unique<EMovement>(EMvtType::LINE_NUM, lineNum);
                    } catch (invalid_argument &e) { }
                }
                try {
                    // can only be some file write thing
                    return make_unique<FileOp>(fileOpMap.at(a->getFragment()));
                }
                catch (out_of_range &e) {
                    throw BadEntry{a->getFragment()};
                }
            } else if (c == KEY_BACKSPACE) {
                if (a->getFragment().size() == 1) throw ClearCmd{};
                a->removeFragment();
                throw UpdateCmd{};
            } else if (isDisplayChar(c)) {
                a->addFragment(static_cast<char>(c));
                throw UpdateCmd{};
            }
            break;
        }
        case IncType::STATIC: {
            if (c == KEY_BACKSPACE) throw ClearCmd{};
            if (c == a->getFragment()[0]) {
                switch (c) {
                    case 'c': return make_unique<Insert>(InsType::CH_LINE, a->getMult());
                    case 'd': return make_unique<TextEdit>(TextEditType::DEL_LINE, a->getMult());
                    case 'y': return make_unique<TextEdit>(TextEditType::YANK_LINE, a->getMult());
                    default: break;
                }
            }
            try {
                switch (a->getFragment()[0]) {
                    case 'c': 
                        return make_unique<Insert>(InsType::CH_MVT, 1, 
                            make_unique<Movement>(mvtMap.at(c), a->getMult()));
                    case 'd': 
                        return make_unique<TextEdit>(TextEditType::DEL_MVT, 1,
                            make_unique<Movement>(mvtMap.at(c), a->getMult()));
                    case 'y': 
                        return make_unique<TextEdit>(TextEditType::YANK_MVT, 1,
                            make_unique<Movement>(mvtMap.at(c), a->getMult()));
                    default: break;
                }
            } catch (out_of_range &e) {
                throw BadEntry{a->getFragment()};
            }
        }
    }
    return unique_ptr<Action>{};
}
/*
unique_ptr<Action> Input::action(ESearch *a) {
    int c = getch();
    
    if (c == ERR) return make_unique<Global>(GlobalType::NONE);
    if (c == ESC)
    // usual text editing stuffs
    // if esc, clear
    // if empty, clear
    if (32 <= c && c <= 126) {
        
    }
}*/

unique_ptr<Action> Input::action(ESearch *a) {
    return unique_ptr<ESearch>();
}

unique_ptr<Action> Input::action(Search *a) {
    return unique_ptr<Search>();
}
}
