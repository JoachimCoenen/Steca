// (dev_lib)

#include "vec.hpp"
#include "../inc/defs.inc"

//------------------------------------------------------------------------------

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
// eof DOCS
