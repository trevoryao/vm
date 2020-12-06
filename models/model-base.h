#ifndef __MODEL_BASE_H__
#define __MODEL_BASE_H__

#include <memory>
#include <string>
#include <vector>

#include "mode-type.h"
#include "../actions/action.h"
#include "../actions/e-search.h"
#include "../actions/incomplete.h"
#include "../actions/search.h"
#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using std::unique_ptr;

namespace models {
class ModelBase {
private:
    std::vector<unique_ptr<views::ViewBase>> views;
    unique_ptr<controllers::ControllerBase> inputController;
    unique_ptr<controllers::ControllerBase> keyController;
protected:
    void addView(unique_ptr<views::ViewBase> v);
    void addInputController(unique_ptr<controllers::ControllerBase> c);
    void addKeyController(unique_ptr<controllers::ControllerBase> c);
    unique_ptr<actions::Action> getAction(ModeType m);
    unique_ptr<actions::Action> getAction(actions::Incomplete *a);
    unique_ptr<actions::Action> getAction(actions::ESearch *a);
    unique_ptr<actions::Action> getAction(actions::Search *a);
public:
    virtual ~ModelBase() = default;
    
    void displayViews();
    void displayInfo();
    
    void resizeViews();
    
    void moveCursor(int y, int x);
    
    int getHeight();
    
    void updateExecView(const std::string &s);
    void updateWarning(const std::string &s);
    void clearExecView();
    
    void updateStaticView(const std::string &s);
    void clearStaticView();
    
    void writeMessage(const std::string &s);
    void writeMode(const std::string &s);
    
    void updateView(std::string &s, int y, int x);
};
}

#endif
