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

#include "ij.h"
#include <c2/c/c_cpp>

core_IJ::core_IJ() : core_IJ(0, 0) {}

core_IJ::core_IJ(int i_, int j_) : i(i_), j(j_) {}

namespace core {
//------------------------------------------------------------------------------

TEST("IJ()",
  IJ ij;
  CHECK_EQ(0, ij.i);
  CHECK_EQ(0, ij.j);
)

TEST("IJ(i,j)",
  IJ ij(2,3);
  CHECK_EQ(2, ij.i);
  CHECK_EQ(3, ij.j);
)

COMPARABLE_IMPL(IJ) {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(IJ)

TEST("IJ::compare",
  IJ ij(1,2), ij1(1,2), ij2(1,0), ij3(2,2);
  CHECK_EQ( 0, ij.compare(ij));
  CHECK_EQ( 0, ij.compare(ij1));
  CHECK_EQ( 1, ij.compare(ij2));
  CHECK_EQ(-1, ij.compare(ij3));

  CHECK_EQ(ij, ij1);
  CHECK_NE(ij, ij2);
)

//------------------------------------------------------------------------------
}
// eof
