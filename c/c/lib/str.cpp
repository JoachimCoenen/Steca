// c

#include "str.h"
#include <c/c/cpp>
#include "unsafe.h"
#include <string.h>

c_str::c_str(sz_t sz_, pcstr p_) : sz(sz_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

str::str(pcstr p) : c_str(p ? strlen(p) : 0, nullptr) {
  mut(p) = static_cast<pstr>(::unsafe::mem::cpy(sz + 1, p ? p : ""));
}

str::str(rc that) : str(that.p) {}

str::str(rval that) : c_str(that.sz, nullptr) {
  unsafe::swap(p, that.p);
}

str::~str() {
  unsafe::mem::free(mut(p));
}


TEST("str",
  str s1(0), s2(""), s3("s");
)

//------------------------------------------------------------------------------
}
// eof
