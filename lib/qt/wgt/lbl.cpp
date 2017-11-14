// (qt_lib)

#include "lbl.hpp"
#include "../defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

lbl::lbl() : Self(str::null) {}

lbl::lbl(strc t) {
  text(t);
}

lbl::ref lbl::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof DOCS
