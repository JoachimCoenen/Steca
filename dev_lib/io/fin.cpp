// (dev_lib)

#include "fin.hpp"
#include <dev_lib/defs.inc>
#include <string.h>

namespace l_io {
//------------------------------------------------------------------------------

buf::buf(sz_t sz) : base(sz, '\0') {}

buf::buf(pcstr p) : base() {
  if (!p)
    p ="";

  auto n = strlen(p);
  resize(n);
  memcpy(data(), p, n);
}

fbin::fbin(path::rc path) : base(path, in | binary), basename(path.basename()) {
  check_or_err_(good(), CAT("cannot open: ", path));
}

void fbin::err(strc msg) const will_err {
  l::err(CAT(basename, ": ", msg));
}

bool fbin::hasMore() {
  base::peek();
  return base::good();
}

uint fbin::read(void* buf, uint n, bool exact) may_err {
  EXPECT_(buf)
  base::read(static_cast<char*>(buf), n);

  auto gc = gcount();
  EXPECT_(gc <= n)
  check_or_err_(good() && (!exact || gc == n),
                CAT("cannot read: ", basename));
  return gc;
}

buf fbin::read(uint n, bool exact) may_err {
  buf b(n);
  n = read(b.data(), n, exact);
  b.resize(n);
  return b;
}

fbin::ref fbin::seek(uint32 pos) may_err {
  base::seekg(pos);
  check_or_err_(good(), CAT("cannot seek: ", basename));
  RTHIS
}

uint8 fbin::get8() may_err {
  uint8 u; read(&u, 1);
  return u;
}

uint16 fbin::get16() may_err {
  uint16 u; read(&u, 2);
  return u;
}

uint32 fbin::get32() may_err {
  uint32 u; read(&u, 4);
  return u;
}

uint64 fbin::get64() may_err {
  uint64 u; read(&u, 8);
  return u;
}

//------------------------------------------------------------------------------
}
// eof
