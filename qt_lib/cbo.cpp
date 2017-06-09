// (app_lib)

#include "cbo.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

cbo::cbo() {}

cbo::cbo(str_lst_rc ts) {
  add(ts);
}

cbo::ref cbo::add(strc t) {
  base::addItem(t);
  return *this;
}

cbo::ref cbo::add(str_lst_rc ts) {
  for (auto t : ts)
    add(t);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
