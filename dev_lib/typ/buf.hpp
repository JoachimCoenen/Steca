// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "vec.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(buf, vec<char>)
  buf(sz_t);
  buf(pcstr, bool zeroByte = true);

  mth_(str, asStr, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
