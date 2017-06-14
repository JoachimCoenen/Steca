// (qt_lib)

#include "rio.h"
#include <dev_lib/inc/defs_cpp.h>
#include "str_inc.h"

namespace l_qt {
//------------------------------------------------------------------------------

rio::rio(strc t) {
  text(t);
}

rio::ref rio::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
