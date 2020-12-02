#include "input.h"

#include <exception>
#include <memory>
#include <ncurses.h>
#include <string>

#include "../actions/action.h"
#include "../actions/bad-entry.h"
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

using namespace actions;
using namespace std;

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
        {'i', ActionType::INS},
        {'a', ActionType::INS},
        {'O', ActionType::INS},
        {'o', ActionType::INS},
        {'I', ActionType::INS},
        {'A', ActionType::INS},
        // {'c', ActionType::INS},
        {'S', ActionType::INS},
        {'C', ActionType::INS},
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
        {'C', InsType::CH}
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
        {'f', ScrollType::PG_FWD},
        {'b', ScrollType::PG_BACK},
        {'d', ScrollType::LINE_FWD},
        {'u', ScrollType::LINE_BACK}
    } { }

unique_ptr<Action> Input::action() {
    int c = getch();

    switch (c) {
        case ERR: return make_unique<Global>(GlobalType::NONE);
        case KEY_RESIZE: return make_unique<Global>(GlobalType::RESIZE);
        case ':': return make_unique<Incomplete>(IncType::EXEC);
        case '/': return make_unique<ESearch>(ESearchType::NEW_FWD_SEARCH);
        case '?': return make_unique<ESearch>(ESearchType::NEW_PREV_SEARCH);
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return make_unique<Incomplete>(IncType::UNKNOWN, c - '0');
        case 'c':
        case 'd':
        case 'y':
            return make_unique<Incomplete>(IncType::STATIC, c);
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
        if (ctrl.size() != 2) return make_unique<Global>(GlobalType::NONE);
        try {
            if (ctrl[1] == 'g') return make_unique<Global>(GlobalType::DISPLAY_FILE);
            return make_unique<Scroll>(scrollMap.at(ctrl[1]));
        } catch (out_of_range &e) {
            return make_unique<Global>(GlobalType::NONE);
        }
    }
}
// throw parse error
// take an incomplete
/*
case 1: exec
push back until enter is pressed, then notify action() that can be executed, notify model
case 2: global
if char, then give back to action so action can make new one
case 3: static
internal count to 5, of no presses, execute whats there
if key is pressed and matches current, then tell action to execute double press
if fails to match, then no execution

globally, return bool:
true => can be executed
false => cannot be executed
*/

// add one for searches
unique_ptr<Action> Input::action(Incomplete *a) {
    int c = getch();
    
    if (c == ERR) {
        return make_unique<Global>(GlobalType::NONE);
    } else if (c == KEY_RESIZE) return make_unique<Global>(GlobalType::RESIZE);
    
    switch (a->getValue()) {
        case IncType::UNKNOWN: {
            if ('0' <= c && c <= '9') {
                a->addFragment(c);
                break;
            } else if (c == 'c' || c == 'd' || c == 'y') {
                return make_unique<Incomplete>(IncType::STATIC, c, stoi(a->getFragment()));
            } else {
                return parseAction(c, stoi(a->getFragment()));
            }
        }
        case IncType::EXEC: {
            if (c == KEY_ENTER) {
                if (a->getFragment().size() > 1) {
                    try {
                        int lineNum = stoi(a->getFragment());
                        if (lineNum == 0) {
                            return make_unique<EMovement>(EMvtType::TOP, a->getMult());
                        }
                        return make_unique<EMovement>(EMvtType::LINE_NUM, a->getMult(), lineNum);
                    } catch (out_of_range &e) { }
                } else if (a->getFragment() == ":$") {
                    return make_unique<EMovement>(EMvtType::BOTTOM, a->getMult());
                }
                
                try {
                    // can only be some file write thing
                    return make_unique<FileOp>(fileOpMap.at(a->getFragment()));
                }
                catch (out_of_range &e) {
                    throw BadEntry{};
                }
            }
        }
        case IncType::STATIC: {
            // c, d, y
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
                throw BadEntry{};
            }
        }
    }
    return make_unique<Global>(GlobalType::NONE);
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
