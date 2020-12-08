#ifndef __TEXT_EDIT_H__
#define __TEXT_EDIT_H__

#include <memory>

#include "movement.h"

using std::unique_ptr;

namespace models {
class TextModel;
}

namespace actions {
enum class TextEditType {
    DEL_LINE,
    DEL_MVT,
    DEL_UNDER,
    DEL_BEFORE,
    PUT_AFTER,
    PUT_BEFORE,
    UNDO,
    REPEAT,
    YANK_LINE,
    YANK_MVT,
    JOIN
};

class TextEdit final : public IAction<TextEditType> {
private:
    unique_ptr<Movement> mvt;
public:
    TextEdit(TextEditType value, int n = 1, unique_ptr<Movement> mvt = unique_ptr<Movement>{});
    
    void execAction(models::TextModel &t) override;
};
}

#endif
