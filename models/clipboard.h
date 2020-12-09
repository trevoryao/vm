#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <memory>

#include "row.h"
#include "../actions/text-edit.h"

namespace models {
class Clipboard final {
private:
    std::unique_ptr<actions::TextEdit> clipboard;
public:
    Clipboard() = default;

    void add(std::unique_ptr<actions::TextEdit> clipboard);
    actions::TextEdit *get();
    explicit operator bool() const;
};
}

#endif
