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
    EMovement(EMvtType value, int mult, int lineNum = 0);
    
    int getLineNum();
    
    void execAction(models::TextModel &t) { }
};
}

#endif
