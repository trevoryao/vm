#ifndef __INPUT_H__
#define __INPUT_H__

#include <map>
#include <memory>

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

#include "controller-base.h"

using actions::Action;
using std::map;
using std::string;

namespace controllers {
class Input final : public ControllerBase {
private:
    enum class ActionType {
        MVT,
        INS,
        REPLACE,
        TEXT_EDIT,
        SEARCH,
        E_SEARCH
    };
    
    map<int, ActionType> actionCharMap;
    map<int, actions::MvtType> mvtMap;
    map<char, actions::InsType> insMap;
    map<char, actions::ReplaceType> replaceMap;
    map<string, actions::FileOpType> fileOpMap;
    map<string, actions::EMvtType> eMvtMap;
    map<char, actions::TextEditType> textEditMap;
    map<char, actions::SearchType> searchMap;
    map<char, actions::ESearchType> eSearchMap;
    map<char, actions::ScrollType> scrollMap;
public:
    Input();
protected:
    std::unique_ptr<Action> action() override;
    std::unique_ptr<Action> action(Incomplete *a) override;
private:
    std::unique_ptr<Action> parseAction(int c, int n);
    std::unique_ptr<Action> parseExec(Incomplete *a);
    std::unique_ptr<Action> parseSearch(Incomplete *a);
};
}

#endif
