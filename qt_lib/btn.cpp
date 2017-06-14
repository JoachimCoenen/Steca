// (qt_lib)

#include "btn.h"
#include <dev_lib/inc/defs_cpp.h>
#include "str_inc.h"

namespace l_qt {
//------------------------------------------------------------------------------

btn::btn(strc t) {
  text(t);
}

btn::ref btn::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
