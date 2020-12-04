#ifndef __E_MOVEMENT_H__
#define __E_MOVEMENT_H__

#include <map>
#include <string>

#include "i-action.h"

namespace models {
class TextModel;
}

namespace actions {
enum class EMvtType { TOP, BOTTOM, LINE_NUM };

class EMovement final : public IAction<EMvtType> {
private:
    int lineNum;
public:
    EMovement(EMvtType value, int lineNum = 0);
    
    void execAction(models::TextModel &t) override;
};
}

#endif
