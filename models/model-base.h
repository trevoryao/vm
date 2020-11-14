#ifndef __MODEL_BASE_H__
#define __MODEL_BASE_H__

#include <memory>
#include <vector>

#include "../controllers/controller-base.h"
#include "../views/view-base.h"

using std::unique_ptr;

namespace vm {
class ModelBase {
private:
    std::vector<unique_ptr<ViewBase>> views;
    std::vector<unique_ptr<ControllerBase>> controllers;
protected:
    void addView(unique_ptr<ViewBase> v);
    void addController(unique_ptr<ControllerBase> c);
public:
    virtual ~ModelBase() = default;
};
}

#endif
