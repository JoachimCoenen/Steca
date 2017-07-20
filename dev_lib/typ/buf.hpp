// (dev_lib)

#pragma once

#include "vec.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(buf, vec<char>)
  buf(sz_t);
  buf(pcstr);

  mth_(str, asStr, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
