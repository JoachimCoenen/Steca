// (qt_lib)

#include "rio.hpp"
#include <dev_lib/defs.inc>

#include "str_inc.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

rio::rio(strc t) {
  text(t);
}

rio::ref rio::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
