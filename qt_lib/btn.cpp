// (qt_lib)

#include "btn.hpp"
#include "inc/defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

btn::btn(strc t) {
  text(t);
}

btn::ref btn::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
