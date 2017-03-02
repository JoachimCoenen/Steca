/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#include "xy.h"
#include <c/c/c_cpp>

core_XY::core_XY() : core_XY(c::NAN, c::NAN)  {}

core_XY::core_XY(real x_, real y_) : x(x_), y(y_) {}

namespace core {
//------------------------------------------------------------------------------

TEST("XY()",
  XY xy;
  CHECK(c::isnan(xy.x));
  CHECK(c::isnan(xy.y));
)

TEST("XY(x,y)",
  XY xy(2.3,3.4);
  CHECK_EQ(2.3, xy.x);
  CHECK_EQ(3.4, xy.y);
)

COMPARABLE_IMPL(XY) {
  EXPECT(isDef() && that.isDef())
  RET_COMPARE_IF_NE_THAT(x)
  RET_COMPARE_IF_NE_THAT(y)
  return 0;
}

DEF_EQ_NE_IMPL(XY)

TEST("XY::compare",
  XY xy(1,2), xy1(1,2), xy2(1,0), xy3(2,2);
  CHECK_EQ( 0, xy.compare(xy));
  CHECK_EQ( 0, xy.compare(xy1));
  CHECK_EQ( 1, xy.compare(xy2));
  CHECK_EQ(-1, xy.compare(xy3));

  CHECK_EQ(xy, xy1);
  CHECK_NE(xy, xy2);
)

bool XY::isDef() const {
  return !(c::isnan(x) || c::isnan(y));
}

TEST("XY::isDef",
  XY undef;
  CHECK(!undef.isDef());
  CHECK(!XY(0,undef.y).isDef());
  CHECK(XY(0,0).isDef());
)

//------------------------------------------------------------------------------
}
// eof
