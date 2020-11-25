#ifndef __STATUS_VIEW_H__
#define __STATUS_VIEW_H__

#include <string>

#include "../models/text-model.h"
#include "view-base.h"

namespace views {
class StatusView : public ViewBase {
private:
    bool hasName;
    models::TextModel &instance;
public:
    StatusView(models::TextModel &instance);
    
    void update(char c, int y, int x) override;
    void update(const std::string &s, int y, int x) override;
    
    void displayView() override;
    
    void resizeView() override;
    
    int getMaxHeight() override;
    int getMaxWidth() override;
    
    void moveCursor(int y, int x) override;
};
}

#endif
