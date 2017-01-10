// c

#include "str.h"
#include "../def/def_cpp"
#include "../def/def_debug"
#include "../test/test.h"
#include <string.h>

namespace c {
//------------------------------------------------------------------------------

str::str(pcstr p) : base(p ? strlen(p) + 1 : 1, p ? p : "") {
}

str::str(rval that) : base(0) {
  swap(that);
}

TEST("str", ({
  str s1(0), s2(""), s3("s");
  TR(s1 << s2 << s3)
});)

//------------------------------------------------------------------------------
}
// eof
