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
#include <c/c/cpp>

namespace core {
//------------------------------------------------------------------------------

IJ::IJ() : IJ(0, 0) {}

TEST("IJ()", ({
  IJ ij;
  CHECK_EQ(0, ij.i);
  CHECK_EQ(0, ij.j);
});)

IJ::IJ(int i_, int j_) : i(i_), j(j_) {}

TEST("IJ(i,j)", ({
  IJ ij(2,3);
  CHECK_EQ(2, ij.i);
  CHECK_EQ(3, ij.j);
});)

int IJ::compare(rc that) const {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(IJ)

TEST("IJ::compare", ({
  IJ ij(1,2), ij1(1,2), ij2(1,0), ij3(2,2);
  CHECK_EQ( 0, ij.compare(ij));
  CHECK_EQ( 0, ij.compare(ij1));
  CHECK_EQ( 1, ij.compare(ij2));
  CHECK_EQ(-1, ij.compare(ij3));

  CHECK_EQ(ij, ij1);
  CHECK_NE(ij, ij2);
});)

//------------------------------------------------------------------------------
}
// eof
