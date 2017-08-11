// (dev_lib)

#pragma once
#include "../defs.hpp"

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
// eof
