// (dev_lib)

#include "xy.hpp"
#include "../inc/defs.hpp"

namespace l {
//------------------------------------------------------------------------------

xy::xy() : xy(flt_nan, flt_nan) {}
xy::xy(real x_, real y_) : x(x_), y(y_) {}

COMPARABLE_IMPL(xy) {
  EXPECT_(isDef() && that.isDef())
  RET_COMPARE_IF_NE_THAT(x)
  RET_COMPARE_IF_NE_THAT(y)
  return 0;
}

DEF_EQ_NE_IMPL(xy)

bool xy::isDef() const {
  return !(isnan(x) || isnan(y));
}

//------------------------------------------------------------------------------

TEST_("xy()",
  xy xy;
  CHECK(isnan(xy.x));
  CHECK(isnan(xy.y));
)

TEST_("xy(x,y)",
  l::xy xy(2.3,3.4);
  CHECK_EQ(2.3, xy.x);
  CHECK_EQ(3.4, xy.y);
)

TEST_("xy::compare",
  l::xy xy(1,2), xy1(1,2), xy2(1,0), xy3(2,2);
  CHECK_EQ( 0, xy.compare(xy));
  CHECK_EQ( 0, xy.compare(xy1));
  CHECK_EQ( 1, xy.compare(xy2));
  CHECK_EQ(-1, xy.compare(xy3));

  CHECK_EQ(xy, xy1);
  CHECK_NE(xy, xy2);
)

TEST_("xy::isDef",
  l::xy undef;
  CHECK(!undef.isDef());
  CHECK(!l::xy(0,undef.y).isDef());
  CHECK(l::xy(0,0).isDef());
)

//------------------------------------------------------------------------------
}
// eof
