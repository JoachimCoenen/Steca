// (qt_lib)

#include "lbl.hpp"
#include <dev_lib/defs.inc>

#include "str_inc.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

lbl::lbl(strc t) {
  text(t);
}

lbl::ref lbl::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
