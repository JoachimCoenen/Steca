// (qt_lib)

#include "cbo.hpp"
#include <dev_lib/defs.inc>

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
