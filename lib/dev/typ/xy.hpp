// (dev_lib)

#pragma once
#include "../defs.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_(xy) COMPARABLE EQ_NE
  atr_(real, x);
  atr_(real, y);

  xy();
  xy(real, real);

  set_(operator=, (rc));

  bol_(isDef, ());

  set_(set, (real, real));
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
