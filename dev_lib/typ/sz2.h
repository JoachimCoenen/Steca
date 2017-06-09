// (dev_lib)

#ifndef DEV_LIB_TYP_SZ2_H
#define DEV_LIB_TYP_SZ2_H

#include "../inc/defs_h.h"

namespace l {
//------------------------------------------------------------------------------
// 2-D size

dcl_(sz2) COMPARABLE EQ_NE
  atr_(uint, i);
  atr_(uint, j);

  sz2();
  sz2(uint, uint);

  bol_(isEmpty, ());

  mth_(sz_t, area,       ());
  mth_(Self, transposed, ());
dcl_end

sz2 operator-(sz2::rc, sz2::rc);

//------------------------------------------------------------------------------
}
#endif
// eof
