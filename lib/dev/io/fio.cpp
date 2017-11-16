// (lib/dev)

#include "fio.hpp"
#include "../inc/defs.inc"

namespace l_io {
//------------------------------------------------------------------------------

fio::fio() {}
fio::~fio() {}

void fio::err(strc msg) const will_err {
  l::err(CAT(basename, ": ", msg));
}

fio::ref fio::setPath(path::rc path) {
  mut(basename) = path.filename(); RTHIS
}

//------------------------------------------------------------------------------

fin::fin() {}

fin::fin(path::rc path, bool binary) {
  open2(path, binary);
}

void fin::open2(path::rc path, bool binary) may_err {
  setPath(path);
  base::open(path, binary ? (base::in | base::binary) : base::in);
  check_or_err_(good(), CAT("cannot open: ", path));
}

void fin::seek2(pos_type pos) may_err {
  base::seekg(pos);
  check_or_err_(good(), CAT("cannot seek: ", basename));
}

bool fin::hasMore() const {
  mut(*this).base::peek();
  return base::good();
}

fin::pos_type fin::tell() const {
  return mut(*this).base::tellg();
}

std::istream& fin::asStream() {
  return mut(base_rc());
}

//------------------------------------------------------------------------------

fbin::fbin() {}
fbin::fbin(path::rc path) : base(path, true) {}

uint fbin::read(void* buf, uint n, bool exact) may_err {
  EXPECT_(buf)
  base::read(static_cast<char*>(buf), n);

  auto gc = gcount();
  EXPECT_(gc <= n)
  check_or_err_(good() && (!exact || gc == n),
                CAT("cannot read: ", basename));
  return l::to_uint(gc);
}

l::buf fbin::read(uint n, bool exact) may_err {
  l::buf b(n);
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
  union {
    uint32 u; flt32 f;
  } u;

  read(&u.u, sizeof (u.u));
  u.u = (endian::le == endianness)
        ? endian::from_le(u.u) : endian::from_be(u.u);
  return u.f;
}

flt64 fbin::getflt64() may_err {
  union {
    uint64 u; flt64 f;
  } u;

  read(&u.u, sizeof (u.u));
  u.u = (endian::le == endianness)
        ? endian::from_le(u.u) : endian::from_be(u.u);
  return u.f;
}

//------------------------------------------------------------------------------

ftin::ftin() {}
ftin::ftin(path::rc path) : base(path, false) {}

str ftin::getLine() may_err {
  str s; std::getline(*this, s);
  check_or_err_(good(), CAT("cannot read: ", basename));
  return s;
}

//------------------------------------------------------------------------------

fout::fout() {}

fout::fout(path::rc path, bool binary) {
  open2(path, binary);
}

void fout::open2(path::rc path, bool binary) may_err {
  setPath(path);
  base::open(path, binary ? (base::out | base::binary) : base::out);
  check_or_err_(good(), CAT("cannot open: ", path));
}

std::ostream& fout::asStream() {
  return mut(base_rc());
}

fbout::fbout(path::rc path) : base(path, true) {}

ftout::ftout(path::rc path) : base(path, true) {}

//------------------------------------------------------------------------------
}
// eof
