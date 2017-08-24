// (qt_lib)

#include "edit.hpp"
#include "inc/defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

edit::edit() {}

edit::ref edit::ro(bool on)
  SET_(base::setReadOnly(on))

edit::ref edit::text(strc t) {
  base::setText(toQt(t));
  base::setCursorPosition(0);
  RTHIS
}

str edit::text() const {
  return fromQt(base::text());
}

//------------------------------------------------------------------------------
}
// eof