// c

#include "str.h"
#include "mem.h"
#include <c/c/cpp>
#include "unsafe.h"
#include <string.h>
#include <cstdarg>
#include <cstdio>

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

str::str(sz_t maxSz, pcstr p_) : c_base(0, nullptr) {
  if (p_) {
    mut(sz) = c::min(maxSz, strlen(p_));
    mut(p) = static_cast<pstr>(::unsafe::mem::cpy(sz + 1, p_));
    mut(*(p+sz)) = '\0';
  } else {
    EXPECT(0 == sz)
    mut(p) = static_cast<pstr>(::unsafe::mem::cpy(1, ""));
  }
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

str str::trim() const {
  pcstr p1 = p, p2 = p + sz; char c;
  while ((c = *p1) && (' '==c || '\t'==c || '\n'==c))
    ++p1;
  while ((p1 < p2) && (' '==(c = *(p2-1)) || '\t'==c || '\n'==c))
    --p2;
  return str(sz_t(p2-p1), p1);
}

str str::format(pcstr f, ...) {
   sz_t sz = 96;  // initial size

   for(;;) {
     mem m(sz+1); pstr p = static_cast<pstr>(mut(m.p));

     va_list marker;
     va_start(marker, f);

     #pragma GCC diagnostic ignored "-Wformat-nonliteral"
     int cnt = vsnprintf(p, sz, f, marker);

     va_end(marker);

     if (cnt < 0)
       return "<encoding error>";

     WT(sz << cnt)
     if (sz < to_u(cnt)) {
       sz = to_u(cnt);
       continue;
     }

     return str(to_u(cnt), p);
   }
 }

TEST("str::trim",
  CHECK_EQ(str("abc"), str(" \nabc \t").trim());
)

TEST("str::format",
  CHECK_EQ(str("abc 2 3 -4"), str::format("abc %d %d %d", 2, 3, -4));
  CHECK_EQ(str("abc 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 429496729"), str::format("abc %u %u %u %u %u %u %u %u %u %u %u %u", -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));
)

//------------------------------------------------------------------------------
}
// eof
