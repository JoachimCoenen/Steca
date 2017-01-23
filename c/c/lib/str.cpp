// c

#include "str.h"
#include "mem.h"
#include "num.h"
#include "../cpp"
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

str::str() : str(nullptr) {}

str::str(pcstr p_) : c_base(p_ ? strlen(p_) : 0, nullptr) {
  mut(p) = static_cast<pstr>(unsafe::memcpy(sz + 1, p_ ? p_ : ""));
}

str::str(sz_t maxSz, pcstr p_) : c_base(0, nullptr) {
  if (p_) {
    mut(sz) = c::min(maxSz, strlen(p_));
    mut(p) = static_cast<pstr>(unsafe::memcpy(sz + 1, p_));
    mut(*(p+sz)) = '\0';
  } else {
    EXPECT(0 == sz)
    mut(p) = static_cast<pstr>(unsafe::memcpy(1, ""));
  }
}

str::str(rc that)
: c_base(that.sz, static_cast<pstr>(unsafe::memcpy(that.sz + 1, that.p))) {}

str::str(rval that) : c_base(that.sz, nullptr) {
  mutate::swap(p, that.p);
}

str::~str() {
  unsafe::free(mut(p));
}

TEST_CODE(
  str str_move(str&& s) { return std::move(s); }
)

TEST("str",
  str s1(0), s2(""), s3("s"), s4(s3), s5(str_move(str("s"))), s6(1, 0), s7(1, "sd");
  CHECK_EQ(s1, s2);
  CHECK_EQ(s1, s6);
  CHECK_NE(s1, s3);
  CHECK_LT(s1, s3);
  CHECK_LE(s1, s3);
  CHECK_EQ(0, strcmp(s3, "s"));
  CHECK_GT(s3, s2);
  CHECK_GE(s3, s2);
  CHECK_EQ(s3, s4);
  CHECK_EQ(s3, s5);
  CHECK_EQ(s3, s7);
)

bool str::empty() const {
  return !*p;
}


bool str::eq(pcstr p_) const {
  if (!p_)
    return empty();
  return 0 == strcmp(p, p_);
}

str::rc str::set(pcstr p) {
  return set(str(p));
}

str::rc str::set(rval that) {
  mutate::swap(sz, that.sz);
  mutate::swap(p,  that.p);
  return *this;
}

str str::trim() const {
  pcstr p1 = p, p2 = p + sz; char c;
  while ((c = *p1) && (' '==c || '\t'==c || '\n'==c))
    ++p1;
  while ((p1 < p2) && (' '==(c = *(p2-1)) || '\t'==c || '\n'==c))
    --p2;
  return str(sz_t(p2-p1), p1);
}

TEST("str::trim",
  CHECK_EQ(str("abc"), str(" \nabc \t").trim());
)

str str::cat(pcstr p1, pcstr p2) {
  if (!p1 || !*p1)
    return str(p2);
  if (!p2 || !*p2)
    return str(p1);

  return unsafe::str_cat(p1, p2, 0);
}

TEST("str::cat",
  CHECK_EQ(str("comes back"), str::cat("comes ", "back"));
)

str const str::nul = "";

TEST("std::nul",
  CHECK_EQ(str(""), str::nul);
)

namespace unsafe {
//------------------------------------------------------------------------------

str str_frm(pcstr f, ...) {
  sz_t sz = 96;  // initial size

  for(;;) {
    mem m(sz+1); pstr p = static_cast<pstr>(mut(m.p));

    va_list marker;
    va_start(marker, f);

    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
    int cnt = vsnprintf(p, sz, f, marker);

    va_end(marker);

    if (cnt < 0)
    return "";

    if (sz < to_u(cnt)) {
    sz = to_u(cnt);
    continue;
    }

    return str(to_u(cnt), p);
  }
}

TEST("str::format",
  CHECK_EQ(str("abc 2 3 -4"), str_frm("abc %d %d %d", 2, 3, -4));
  CHECK_EQ(str("abc 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 4294967295 429496729"), str_frm("abc %u %u %u %u %u %u %u %u %u %u %u %u", -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));
)

#ifndef _MSC_VER
TEST("str::format-error",
  CHECK_EQ(str(""), str_frm("abc %-", 5)); // encoding error
)
#endif

str str_cat(pcstr p, ...) {
  EXPECT(p)

  va_list marker;
  va_start(marker, p); pcstr pp = p;

  sz_t sz = 0;
  for (;;) {
    sz += strlen(pp);
    pp = va_arg(marker, pcstr);
    if (!pp) break;
  }

  va_end(marker);

  mem m(sz + 1);
  pstr s = static_cast<pstr>(mut(m.p)), sp = s;

  va_start(marker, p); pp = p;

  for (;;) {
    strcpy(sp, pp);
    sp += strlen(pp);
    pp = va_arg(marker, pcstr);
    if (!pp) break;
  }

  va_end(marker);

  return str(s);
}

//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
}
// eof
