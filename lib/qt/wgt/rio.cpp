// (qt_lib)

#include "rio.hpp"
#include "../defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

rio::rio(strc t) {
  text(t);
}

rio::ref rio::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof DOCS
