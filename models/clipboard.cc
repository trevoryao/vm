#include "clipboard.h"

#include <memory>
#include <utility>

#include "row.h"
#include "../actions/text-edit.h"

using namespace actions;
using namespace std;

namespace models {
void Clipboard::add(unique_ptr<TextEdit> c) { clipboard = move(c); }

TextEdit *Clipboard::get() { return clipboard.get(); }

Clipboard::operator bool() const { return clipboard.get(); }
}
