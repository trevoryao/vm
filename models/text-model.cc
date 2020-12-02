#include "text-model.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "mode-type.h"
#include "../actions/action.h"
#include "../actions/e-movement.h"
#include "../actions/e-search.h"
#include "../actions/file-op.h"
#include "../actions/global.h"
#include "../actions/incomplete.h"
#include "../actions/insert.h"
#include "../actions/movement.h"
#include "../actions/replace.h"
#include "../actions/scroll.h"
#include "../actions/search.h"
#include "../actions/text-edit.h"
#include "../controllers/input.h"
#include "../views/status-view.h"
#include "../views/text-view.h"

using namespace actions;
using namespace controllers;
using namespace std;
/*
namespace {
inline bool isWordChar(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') ||
        ('0' <= c && '9') || (c == '_');
}
}*/

namespace models {
TextModel::TextModel(const string &fileName) : fileName{fileName}, text{fileName}, 
    mode{ModeType::CMD}, topLine{0}, curY{0}, curX{0}, runLoop{true} {
    initscr();
	raw();
	noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    addView(make_unique<views::StatusView>(*this));
    addInputController(make_unique<controllers::Input>());
    
    // construct text based on view
    unique_ptr<views::TextView> textView = make_unique<views::TextView>(*this);
    text.resizeText(textView->getMaxWidth());
    addView(std::move(textView));
}

const string &TextModel::getName() { return fileName; }

const vector<string> &TextModel::getText() { return text.getText(); }

void TextModel::getCursor(int &y, int &x) {
    y = curY;
    x = curX;
}

void TextModel::setMaxY(int y) { maxY = y; }
void TextModel::setMaxX(int x) { maxX = x; }

void TextModel::run() {
    displayViews();
    
    while (runLoop) {
        auto action = getAction(mode);
        switch (action->getType()) {
            case ActionType::E_MVT: break;
            case ActionType::E_SEARCH: break;
            case ActionType::FILE_OP: break;
            case ActionType::GLOBAL: execAction(static_cast<Global *>(action.get())); break;
            case ActionType::INCOMPLETE: execAction(static_cast<Incomplete *>(action.get())); break;
            case ActionType::INS: break;
            case ActionType::KEYBOARD: break;
            case ActionType::MVT: execAction(static_cast<Movement *>(action.get())); break;
            case ActionType::REPLACE: break;
            case ActionType::SCROLL: break;
            case ActionType::SEARCH: break;
            case ActionType::TEXT_EDIT: break;
        }
    }
}

void TextModel::resizeText(int newMaxX) { text.resizeText(newMaxX); }

void TextModel::execAction(Global *a) {
    switch (a->getValue()) {
        case GlobalType::NONE: break;
        case GlobalType::RESIZE: resizeViews(); break;
        case GlobalType::DISPLAY_FILE: break; // TODO
    }
}

void TextModel::execAction(Incomplete *a) {
    if (a->getValue() == IncType::EXEC) {
        
    } else {
        // clear, update
    }
}

void TextModel::execAction(Movement *a) {
    switch (a->getValue()) {
        case MvtType::LEFT: moveLeft(a->getMult()); break;
        case MvtType::RIGHT: moveRight(a->getMult()); break;
        case MvtType::UP: moveUp(a->getMult()); break;
        case MvtType::DOWN: moveDown(a->getMult()); break;
        case MvtType::WORD_LEFT: break;
        case MvtType::WORD_RIGHT: break;
        case MvtType::BEG_LINE: moveLeft(curX); break;
        case MvtType::BEG_CH: break;
        case MvtType::END_CH: break;
    }
}

void TextModel::moveLeft(int n) {
    moveCursor(curY, curX - n < 0 ? 0 : curX - n);
    curX = curX - n < 0 ? 0 : curX - n;
}

void TextModel::moveRight(int n) {
    int max = text.getText()[curY].size() - (mode == ModeType::CMD ? 2 : 1);
    moveCursor(curY, curX + n > max ? max : curX + n);
    curX = curX + n > max ? max : curX + n;
}

void TextModel::moveUp(int n) {
    // no scroll first, scroll later
     if (topLine < curY) {
        int x = text.getText()[curY - n].size() - (mode == ModeType::CMD ? 2 : 1);
        if (curX > x) {
            if (x < 0) x = 0;
            curX = x;
        }
        moveCursor(curY - n, curX);
        curY -= n;
    }
}

void TextModel::moveDown(int n) {
    if (curY < botLine) {
        int x = text.getText()[curY + 1].size() - (mode == ModeType::CMD ? 2 : 1);
        if (curX > x) {
            if (x < 0) x = 0;
            curX = x;
        }
        moveCursor(curY + n, curX);
        curY += n;
    }
}

void TextModel::searchWordLeft(int n) {
    /*
    int found = 0;
    int y = curY;
    int x = curX;
    while (y >= 0) {
        while (x >= 0) {
            if ()
        }
    }*/
}

TextModel::~TextModel() { endwin(); }
}
