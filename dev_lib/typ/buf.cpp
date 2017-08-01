// (dev_lib)

#include "buf.hpp"
#include "../defs.inc"
#include <string.h>

namespace l {
//------------------------------------------------------------------------------

buf::buf(sz_t sz) : base(sz, '\0') {}

buf::buf(pcstr p) : base() {
  if (!p)
    p = "";

  auto n = strlen(p) + 1;
  resize(n);
  memcpy(data(), p, n);
}

str buf::asStr() const {
  return str(data(), size());
}

//------------------------------------------------------------------------------
}
// eof
