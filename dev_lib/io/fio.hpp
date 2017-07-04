// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "path.hpp"
#include "../typ/vec.hpp"
#include <fstream>

namespace l_io {
//------------------------------------------------------------------------------

dcl_sub_(buf, l::vec<char>)
  buf(sz_t);
  buf(pcstr);
dcl_end

dcl_reimpl_(fbin, std::fstream)
  atr_(str, basename);
  fbin(path::rc);

  mth_(void, check, ()) may_err;
  mth_mut_(buf, read, (uint n));
dcl_end

//------------------------------------------------------------------------------
}
// eof
