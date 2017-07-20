// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "../typ/buf.hpp"
#include "endian.hpp"
#include "path.hpp"
#include <fstream>

namespace l_io {
//------------------------------------------------------------------------------

dcl_reimpl_(fin, std::fstream)
  atr_(str, basename);

  [[noreturn]] mth_(void, err, (strc msg)) will_err;

  mth_mut_(bool, hasMore, ());

  mth_mut_(pos_type, tell, ());

protected:
  fin(path::rc, bool binary);
  set_(seek, (pos_type)) may_err;
dcl_end

dcl_sub_(fbin, fin)
  atr_(endian::ness, endianness) = endian::le;

  fbin(path::rc);

  set_(seek, (pos_type pos)) may_err SET_(base::seek(pos);)

  mth_mut_(uint,   read, (void*, uint n, bool exact = true)) may_err;
  mth_mut_(l::buf, read, (uint n, bool exact = true))        may_err;

  mth_mut_(uint8,  get8,     ()) may_err;
  mth_mut_(uint16, get16,    ()) may_err;
  mth_mut_(uint32, get32,    ()) may_err;
  mth_mut_(uint64, get64,    ()) may_err;
  mth_mut_(flt32,  getflt32, ()) may_err;
  mth_mut_(flt64,  getflt64, ()) may_err;

dcl_end

dcl_sub_(ftin, fin)
  ftin(path::rc);

  set_(seek, (pos_type pos)) may_err SET_(base::seek(pos);)

  mth_mut_(str, getline, ()) may_err;

dcl_end

//------------------------------------------------------------------------------
}
// eof
