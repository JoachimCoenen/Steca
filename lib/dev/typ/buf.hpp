// (lib/dev)

/** @file
 * a data buffer
 */

#pragma once

#include "../defs.hpp"
#include "vec.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(buf, vec<char>)
  buf(uint);
  buf(pcstr, bool zeroByte = true);

  mth_(str, asStr, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
