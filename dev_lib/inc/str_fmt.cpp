// (dev_lib)

#include "str_fmt.h"
#include "../inc/defs_cpp.h"

namespace l {
//------------------------------------------------------------------------------

str trim(strc s) {
  pcstr p1 = s.c_str(), p2 = p1 + s.length();
  while (p1 < p2 && std::isspace(*p1))
    ++p1;
  while (p1 < p2 && std::isspace(*p2))
    ++p2;
  return str(p1, p2-p1);
}

//------------------------------------------------------------------------------
}
// eof
