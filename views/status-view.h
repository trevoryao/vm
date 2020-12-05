#ifndef __STATUS_VIEW_H__
#define __STATUS_VIEW_H__

#include <string>

#include "../models/text-model.h"
#include "view-base.h"

namespace views {
class StatusView final : public ViewBase {
private:
    models::TextModel &instance;
    int div1, div2;
public:
    StatusView(models::TextModel &instance);
    
    void update(char c, int y, int x) override;
    void update(const std::string &s, int y, int x) override;
    
    void updateExec(const std::string &s) override;
    void clearExec() override;
    
    void updateStatic(const std::string &s) override;
    void clearStatic() override;
    
    void writeMessage(const std::string &s) override;
    void writeWarning(const std::string &s) override;
    
    void displayView() override;
    
    void resizeView() override;
    
    int getMaxHeight() override;
    int getMaxWidth() override;
    
    void moveCursor(int y, int x) override;
};
}

#endif
