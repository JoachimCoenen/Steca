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

#include "geometry.hpp"
#include <c2/h/c_cpp>

namespace core {
//------------------------------------------------------------------------------

real const Geometry::MIN_DETECTOR_DISTANCE   = 10;
real const Geometry::MIN_DETECTOR_PIXEL_SIZE = .1;

real const Geometry::DEF_DETECTOR_DISTANCE   = 1035;
real const Geometry::DEF_DETECTOR_PIXEL_SIZE = 1;

Geometry::Geometry()
: detectorDistance(DEF_DETECTOR_DISTANCE), pixSize(DEF_DETECTOR_PIXEL_SIZE)
, midPixOffset() {
}

COMPARABLE_IMPL(Geometry) {
  RET_COMPARE_IF_NE_THAT(detectorDistance)
  RET_COMPARE_IF_NE_THAT(pixSize)
  RET_COMPARABLES_IF_NE(midPixOffset)
  return 0;
}

EQ_NE_IMPL(Geometry)

//------------------------------------------------------------------------------

ImageCut::ImageCut() : ImageCut(0, 0, 0, 0) {
}

ImageCut::ImageCut(uint left_, uint top_, uint right_, uint bottom_)
: left(left_), top(top_), right(right_), bottom(bottom_) {
}

int ImageCut::compare(rc that) const {
  RET_COMPARE_IF_NE_THAT(left)
  RET_COMPARE_IF_NE_THAT(top)
  RET_COMPARE_IF_NE_THAT(right)
  RET_COMPARE_IF_NE_THAT(bottom)
  return 0;
}

EQ_NE_IMPL(ImageCut)

c::sz2 ImageCut::marginSize() const {
  return c::sz2(left + right, top + bottom);
}

//------------------------------------------------------------------------------
}
// eof
