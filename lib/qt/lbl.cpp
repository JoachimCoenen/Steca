// (qt_lib)

#include "lbl.hpp"
#include "inc/defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

lbl::lbl(strc t) {
  text(t);
}

lbl::ref lbl::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof