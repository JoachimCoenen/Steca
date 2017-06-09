// (dev_lib)

#include "sz2.h"
#include "../inc/defs_cpp.h"

namespace l {
//------------------------------------------------------------------------------

sz2::sz2() : Self(0, 0) {}
sz2::sz2(uint i_, uint j_) : i(i_), j(j_) {}

TEST_("sz2",
  sz2 sz0;
  CHECK_EQ(0, sz0.i); CHECK_EQ(0, sz0.j);

  sz2 sz12(1, 2);
  CHECK_EQ(1, sz12.i); CHECK_EQ(2, sz12.j);
)

COMPARABLE_IMPL(sz2) {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(sz2)

TEST_("sz2",
  sz2 sz0, sz00(0, 0), sz12(1, 2);
  CHECK_EQ(sz0, sz00); CHECK_NE(sz00, sz12); CHECK_NE(sz12, sz0);
)

bool sz2::isEmpty() const {
  return 0==i || 0==j;
}

sz_t sz2::area() const {
  return i * j;
}

sz2 sz2::transposed() const {
  return sz2(j, i);
}

TEST_("sz2",
  sz2 sz0, sz01(0, 1), sz10(1, 0), sz12(1, 2);

  CHECK(sz0.isEmpty()); CHECK(sz01.isEmpty()); CHECK(sz10.isEmpty());
  CHECK(!sz12.isEmpty());

  CHECK_EQ(0, sz0.area()); CHECK_EQ(0, sz01.area()); CHECK_EQ(0, sz10.area());
  CHECK_EQ(2, sz12.area());

  CHECK_EQ(sz01, sz10.transposed());
)

sz2 operator-(sz2::rc s1, sz2::rc s2) {
  auto i = to_i(s1.i) - to_i(s2.i); if (i < 0) i = 0;
  auto j = to_i(s1.j) - to_i(s2.j); if (j < 0) j = 0;
  return sz2(to_u(i), to_u(j));
}

TEST_("sz2",
  sz2 sz43(4,3), sz34(3,4);
  CHECK_EQ(sz2(4,3), sz43 - sz2());
  CHECK_EQ(sz2(1,0), sz43 - sz34);
  CHECK_EQ(sz2(0,1), sz34 - sz43);
)

//------------------------------------------------------------------------------
}
// eof
