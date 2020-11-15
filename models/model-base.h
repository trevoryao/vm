#ifndef __MODEL_BASE_H__
#define __MODEL_BASE_H__

#include <memory>
#include <vector>

#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using std::unique_ptr;

namespace vm::models {
class ModelBase {
private:
    std::vector<unique_ptr<views::ViewBase>> views;
    std::vector<unique_ptr<controllers::ControllerBase>> controllers;
protected:
    void addView(unique_ptr<views::ViewBase> v);
    void addController(unique_ptr<controllers::ControllerBase> c);
public:
    virtual ~ModelBase() = default;
    void displayViews();
};
}

#endif