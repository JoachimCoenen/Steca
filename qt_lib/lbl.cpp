// (app_lib)

#include "lbl.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

lbl::lbl(strc t) {
  text(t);
}

lbl::ref lbl::text(strc t) {
  base::setText(t);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
