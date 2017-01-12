// c

#include "str.h"
#include "unsafe.h"
#include "../def/def_cpp"
#include "../test/test.h"
#include <string.h>

//------------------------------------------------------------------------------

_c_con_impl(str, (pcstr p_)) : size(p_ ? strlen(p_) : 0), p(nullptr) {
  mut(p) = static_cast<pstr>(::unsafe::mem::cpy(size + 1, p_ ? p_ : ""));
}

_c_des_impl(str) {
  unsafe::mem::free(zerop(p));
}

namespace c {
//------------------------------------------------------------------------------

str::str(pcstr p) : c_str(p) {
}

str::str(rc that) : str(that.p) {
}

str::str(rval that) : c_str(nullptr) {
  unsafe::swap(size, that.size);
  unsafe::swap(p,    that.p);
}

TEST("str", ({
  str s1(0), s2(""), s3("s");
});)

//------------------------------------------------------------------------------
}
// eof
