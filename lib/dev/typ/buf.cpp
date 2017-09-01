// (dev_lib)

#include "buf.hpp"
#include "../inc/defs.inc"
#include <string.h>

namespace l {
//------------------------------------------------------------------------------

buf::buf(uint sz) : base(sz, '\0') {}

buf::buf(pcstr p, bool zeroByte) : base() {
  if (!p)
    p = "";

  auto n = strlen(p) + (zeroByte ? 1 : 0);
  resize(n);
  memcpy(data(), p, n);
}

str buf::asStr() const {
  return str(data(), size());
}

//------------------------------------------------------------------------------
}
// eof
