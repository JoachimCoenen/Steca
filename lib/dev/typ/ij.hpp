// (lib/dev)

/** @file
 * two-dimensional integral coordinates; signed and unsigned
 */
#pragma once

#include "../defs.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_(ij) COMPARABLE EQNE
  atr_(int, i);
  atr_(int, j);

  ij();
  ij(int, int);
dcl_end

dcl_(uij) COMPARABLE EQNE
  atr_(uint, i);
  atr_(uint, j);

  uij();
  uij(uint, uint);
dcl_end

//------------------------------------------------------------------------------
}
// eof
