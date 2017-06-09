// (dev_lib)

#ifndef DEV_LIB_TYP_IJ_H
#define DEV_LIB_TYP_IJ_H

#include "../inc/defs_h.h"

namespace l {
//------------------------------------------------------------------------------

dcl_(ij) COMPARABLE EQ_NE
  atr_(int, i);
  atr_(int, j);

  ij();
  ij(int, int);
dcl_end

dcl_(uij) COMPARABLE EQ_NE
  atr_(uint, i);
  atr_(uint, j);

  uij();
  uij(uint, uint);
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
