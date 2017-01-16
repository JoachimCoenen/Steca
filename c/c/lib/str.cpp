// c

#include "str.h"
#include <c/c/cpp>
#include "unsafe.h"
#include <string.h>

c_str::c_str(sz_t sz_, pcstr p_) : sz(sz_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

COMPARABLE_IMPL(str) {
  return strcmp(pcstr(*this), pcstr(that));
}

COMP_OPS_IMPL(str)

str::str(pcstr p_) : c_base(p_ ? strlen(p_) : 0, nullptr) {
  mut(p) = static_cast<pstr>(::unsafe::mem::cpy(sz + 1, p_ ? p_ : ""));
}

str::str(rc that) : str(pcstr(that)) {}

str::str(rval that) : c_base(that.sz, nullptr) {
  unsafe::swap(p, that.p);
}

str::~str() {
  unsafe::mem::free(mut(p));
}

TEST("str",
  str s1(0), s2(""), s3("s");
  CHECK_EQ(s1, s2);
  CHECK_NE(s1, s3);
  CHECK_LT(s1, s3);
  CHECK_LE(s1, s3);
  CHECK_GT(s3, s2);
  CHECK_GE(s3, s2);
)

//------------------------------------------------------------------------------
}
// eof
