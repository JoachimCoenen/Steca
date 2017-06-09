// (dev_lib)

#ifndef DEV_LIB_TYP_XY_H
#define DEV_LIB_TYP_XY_H

#include "../inc/defs_h.h"

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
#endif
// eof
