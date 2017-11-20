// (lib/dev)

#include "vecs.hpp"
#include "defs.inc"

//------------------------------------------------------------------------------

str_vec::str_vec(pcstr p[], uint n) {
  str_vec vec;
  while (n-- > 0)
    vec.add(str(*p++));
}

str_vec str_vec::split(strc s) {
  str_vec vec;

  std::stringstream splitter;
  splitter << s;
  while (splitter.good()) {
    str token; splitter >> token;
    if (!token.isEmpty())
      vec.add(token);
  }

  return vec;
}

//------------------------------------------------------------------------------
// eof
