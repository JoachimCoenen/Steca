// (lib/dev)

/** @file
 * 2-dimensional size
 */

#pragma once

#include "../defs.hpp"

namespace l {
//------------------------------------------------------------------------------
// 2-D size

dcl_(sz2) COMPARABLE EQNE
  atr_(uint, i);
  atr_(uint, j);

  sz2();
  sz2(uint, uint);

  set_(operator=, (rc));

  bol_(isEmpty, ());

  mth_(sz_t, size,       ());
  mth_(Self, transposed, ());
dcl_end

sz2 operator-(sz2::rc, sz2::rc);

//------------------------------------------------------------------------------
}
// eof
