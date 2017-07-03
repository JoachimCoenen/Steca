// (dev_lib)

#pragma once

#include "../defs.inc"

namespace l {
//------------------------------------------------------------------------------

dcl_(xy) COMPARABLE EQ_NE
  atr_(real, x);
  atr_(real, y);

  xy();
  xy(real, real);

  bol_(isDef, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
