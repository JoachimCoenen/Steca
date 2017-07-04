// (dev_lib)

#include "fio.hpp"
#include <dev_lib/defs.inc>
#include <string.h>

namespace l_io {
//------------------------------------------------------------------------------

buf::buf(sz_t sz) : base(sz, '\0') {}

buf::buf(pcstr p) : base() {
  if (!p)
    p ="";

  auto n = strlen(p);
  resize(n + 1);
  strcpy(data(), p);
}

fbin::fbin(path::rc path) : base(path, in | binary), basename(path.basename()) {
  check_or_err_(good(), CAT("cannot open: ", path));
}

void fbin::check() const may_err {
  check_or_err_(!bad(), CAT("bad content in: ", basename));
}

buf fbin::read(uint n) {
  buf b(n);
  base::read(b.data(), n);

  auto gc = gcount();
  EXPECT_(gc <= n)
  b.resize(gc);

  return b;
}

//------------------------------------------------------------------------------
}
// eof
