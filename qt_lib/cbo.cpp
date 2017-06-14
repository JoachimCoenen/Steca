// (qt_lib)

#include "cbo.h"
#include <dev_lib/inc/defs_cpp.h>

namespace l_qt {
//------------------------------------------------------------------------------

cbo::cbo() {}

cbo::cbo(str_vec::rc ts) {
  add(ts);
}

cbo::ref cbo::add(strc t) SET_(base::addItem(toQt(t)))

cbo::ref cbo::add(str_vec::rc ts) SET_(for (auto t : ts) add(t))

//------------------------------------------------------------------------------
}
// eof
