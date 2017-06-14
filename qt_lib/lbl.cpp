// (qt_lib)

#include "lbl.h"
#include <dev_lib/inc/defs_cpp.h>
#include "str_inc.h"

namespace l_qt {
//------------------------------------------------------------------------------

lbl::lbl(strc t) {
  text(t);
}

lbl::ref lbl::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
