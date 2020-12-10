#ifndef __TEXT_EDIT_H__
#define __TEXT_EDIT_H__

#include <memory>
#include <vector>

#include "movement.h"
#include "../models/row.h"

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
    std::vector<int> posns;
    std::vector<models::Row> rows;
    unique_ptr<TextEdit> clipboard;
public:
    TextEdit(TextEditType value, int n = 1, unique_ptr<Movement> mvt = unique_ptr<Movement>{});
    
    void execAction(models::TextModel &t) override;
    void undoAction(models::TextModel &t, int y, int x) override;
private:
    unique_ptr<TextEdit> clone();
    
    void delMvt(models::TextModel &t, int y, int x);
    void undoDelMvt(models::TextModel &t, int y, int x);
    
    void yankMvt(models::TextModel &t, int y, int x);
    
    void putBefore(models::TextModel &t, int y, int x);
    void putLinesBefore(models::TextModel &t, int y, int x);
    void putLineBefore(models::TextModel &t, int y, int x);
    
    void putAfter(models::TextModel &t, int y, int x);
    void putLinesAfter(models::TextModel &t, int y, int x);
    void putLineAfter(models::TextModel &t, int y, int x);
    
    void putLinesMiddle(models::TextModel &t, int y, int x);
    
    void undoPut(models::TextModel &t, int y, int x);
};
}

#endif
