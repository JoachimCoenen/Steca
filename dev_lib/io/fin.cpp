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

str buf::asStr() const {
  return str(data(), size());
}

//------------------------------------------------------------------------------

fin::fin(path::rc path, bool binary)
: base(path, binary ? (base::in | base::binary) : base::in)
, basename(path.basename()) {
  check_or_err_(good(), CAT("cannot open: ", path));
}

void fin::err(strc msg) const will_err {
  l::err(CAT(basename, ": ", msg));
}

bool fin::hasMore() {
  base::peek();
  return base::good();
}

fin::pos_type fin::tell() {
  return base::tellg();
}

fin::ref fin::seek(pos_type pos) may_err {
  base::seekg(pos);
  check_or_err_(good(), CAT("cannot seek: ", basename));
  RTHIS
}

//------------------------------------------------------------------------------

fbin::fbin(path::rc path) : base(path, true) {}

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

uint8 fbin::get8() may_err {
  uint8 u; read(&u, 1);
  return u;
}

uint16 fbin::get16() may_err {
  uint16 u; read(&u, 2);
  return (endian::le == endianness)
      ? endian::from_le(u) : endian::from_be(u);
}

uint32 fbin::get32() may_err {
  uint32 u; read(&u, 4);
  return (endian::le == endianness)
      ? endian::from_le(u) : endian::from_be(u);
}

uint64 fbin::get64() may_err {
  uint64 u; read(&u, 8);
  return (endian::le == endianness)
      ? endian::from_le(u) : endian::from_be(u);
}

flt32 fbin::getflt32() may_err {
  uint32 u; read(&u, 4);
  u = (endian::le == endianness)
        ? endian::from_le(u) : endian::from_be(u);
  return *reinterpret_cast<flt32*>(&u);
}

flt64 fbin::getflt64() may_err {
  uint64 u; read(&u, 4);
  u = (endian::le == endianness)
        ? endian::from_le(u) : endian::from_be(u);
  return *reinterpret_cast<flt64*>(&u);
}

//------------------------------------------------------------------------------

ftin::ftin(path::rc path) : base(path, false) {}

str ftin::getline() may_err {
  str s; std::getline(*this, s);
  check_or_err_(good(), CAT("cannot read: ", basename));
  return s;
}

//------------------------------------------------------------------------------
}
// eof
