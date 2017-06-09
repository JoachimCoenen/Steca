// (app_lib)

#include "edit.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

edit::edit() {}

edit::ref edit::ro(bool on) {
  base::setReadOnly(on);
  return *this;
}

edit::ref edit::text(strc t) {
  base::setText(t);
  base::setCursorPosition(0);
  return *this;
}

str edit::text() const {
  return base::text();
}

//------------------------------------------------------------------------------
}
// eof
