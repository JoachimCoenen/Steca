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

  [[noreturn]] mth_(void, err, (strc msg)) will_err;

  mth_mut_(bool, hasMore, ());
  mth_mut_(uint, read, (void*, uint n, bool exact = true)) may_err;
  mth_mut_(buf,  read, (uint n, bool exact = true))        may_err;

  mth_mut_(uint8,  get8,  ()) may_err;
  mth_mut_(uint16, get16, ()) may_err;
  mth_mut_(uint32, get32, ()) may_err;
  mth_mut_(uint64, get64, ()) may_err;
dcl_end

//------------------------------------------------------------------------------
}
// eof
