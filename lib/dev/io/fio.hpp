// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../typ/buf.hpp"
#include "endian.hpp"
#include "path.hpp"
#include <fstream>

namespace l_io {
//------------------------------------------------------------------------------

dcl_base_(fio)
  atr_(str, basename);

  virtual void open2(path::rc, bool binary) may_err = 0;
  [[noreturn]] voi_(err, (strc msg)) will_err;

protected:
  fio();
  set_(setPath, (path::rc));
dcl_end

//------------------------------------------------------------------------------

dcl_reimpl2_(fin, fio, std::fstream)
  fin();
  fin(path::rc, bool binary);

  bol_(hasMore, ());
  mth_(pos_type, tell, ());

  mth_mut_(std::istream&, asStream, ());

protected:
  void open2(path::rc, bool binary) may_err;
  void seek2(pos_type) may_err;
dcl_end

dcl_sub_(fbin, fin)
  atr_(endian::ness, endianness) = endian::le;

  fbin();
  fbin(path::rc);

  set_(open, (path::rc path)) may_err SET_(base::open2(path, true))
  set_(seek, (pos_type pos))  may_err SET_(base::seek2(pos))

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
  ftin();
  ftin(path::rc);

  set_(open, (path::rc path)) may_err SET_(base::open2(path, false))
  set_(seek, (pos_type pos))  may_err SET_(base::seek2(pos))

  mth_mut_(str, getLine, ()) may_err;
dcl_end

//------------------------------------------------------------------------------

dcl_reimpl2_(fout, fio, std::fstream)
  fout();
  fout(path::rc, bool binary);

  void open2(path::rc, bool binary) may_err;

  mth_mut_(std::ostream&, asStream, ());
dcl_end

dcl_sub_(fbout, fout)
  fbout(path::rc);
dcl_end

dcl_sub_(ftout, fout)
  ftout(path::rc);
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
