// (app_lib)

#include "btn.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

btn::btn(strc t) {
  text(t);
}

btn::ref btn::text(strc t) {
  base::setText(t);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
