// (app_lib)

#include "rio.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

rio::rio() : rio(str::null) {}

rio::rio(strc t) {
  text(t);
}

rio::ref rio::text(strc t) {
  base::setText(t);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
