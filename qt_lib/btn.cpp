// (qt_lib)

#include "btn.hpp"
#include <dev_lib/inc/defs.inc>

#include "str_inc.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

btn::btn(strc t) {
  text(t);
}

btn::ref btn::text(strc t) SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------
}
// eof
