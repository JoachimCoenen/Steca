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

#include "angles.hpp"
#include <c2/h/c_cpp>
#include <algorithm>

namespace core {
//------------------------------------------------------------------------------

Angles::Angles() : Angles(0, 0) {}

Angles::Angles(tth_t tth_, gma_t gma_) : tth(tth_), gma(gma_) {}

Angles::ref Angles::operator=(rc that) {
  mut(tth) = that.tth;
  mut(gma) = that.gma;
  return *this;
}

AngleMap::Key0::Key0()
: geometry(), size(), cut(), midPix() {}

AngleMap::Key0::Key0(Geometry::rc geometry_, c::sz2::rc size_,
                     ImageCut::rc cut_, c::ij::rc midPix_)
  : geometry(geometry_), size(size_), cut(cut_), midPix(midPix_) {}

COMPARABLE_IMPL(AngleMap::Key0) {
  RET_COMPARABLES_IF_NE(geometry)
  RET_COMPARABLES_IF_NE(size)
  RET_COMPARABLES_IF_NE(cut)
  RET_COMPARABLES_IF_NE(midPix)
  return 0;
}

EQ_NE_IMPL(AngleMap::Key0)

AngleMap::Key::Key(Geometry::rc geometry_, c::sz2::rc size_,
                   ImageCut::rc cut_, c::ij::rc midPix_, tth_t midTth_)
: base(geometry_, size_, cut_, midPix_), midTth(midTth_) {}

AngleMap::Key::Key(Key0 k, tth_t midTth)
: Key(k.geometry, k.size, k.cut, k.midPix, midTth) {}

COMPARABLE_IMPL(AngleMap::Key) {
  for (int cmp = base::compare(that); cmp; )
    return cmp;
  RET_COMPARE_IF_NE_THAT(midTth)
  return 0;
}

COMP_OPS_IMPL(AngleMap::Key)

//------------------------------------------------------------------------------

AngleMap::AngleMap(Key::rc key_) : key(key_), angles(new angle_arr(c::sz2())) {
  calculate();
}

Angles::rc AngleMap::at(uint i, uint j) const {
  return angles->at(i, j);
}

Angles::rc AngleMap::at(uint i) const {
  return angles->at(i);
}

static uint lowerBound(c::vec<gma_t>::rc vec, gma_t x, uint i1, uint i2) {
  EXPECT (i1 < i2)

  if (1 == i2-i1)
    return i1;

  uint mid = (i1 + i2) / 2;
  return vec.at(mid-1) < x            // x may be NaN ...
      ? lowerBound(vec, x, mid, i2)
      : lowerBound(vec, x, i1, mid);  // ... we should be so lucky
}

static uint upperBound(c::vec<gma_t>::rc vec, gma_t x, uint i1, uint i2) {
  EXPECT (i1 < i2)

  if (1 == i2-i1)
    return i2;

  uint mid = (i1 + i2) / 2;
  return vec.at(mid) > x              // x may be NaN ...
      ? upperBound(vec, x, i1, mid)
      : upperBound(vec, x, mid, i2);  // ... we should be so lucky
}

void AngleMap::getGmaIndexes(gma_rge::rc rgeGma,
                             uint_vec const*& indexes, uint& minIndex, uint& maxIndex) const {
  indexes = &gmaIndexes;
  minIndex = lowerBound(gmas, rgeGma.min, 0, c::to_uint(gmas.size()));
  maxIndex = upperBound(gmas, rgeGma.max, 0, c::to_uint(gmas.size()));
}

void AngleMap::calculate() {
  auto& geometry = key.geometry;
  auto& size     = key.size;
  auto& cut      = key.cut;
  auto& midPix   = key.midPix;
  auto& midTth   = key.midTth;

  real pixSize = geometry.pixSize,
       detDist = geometry.detectorDistance;

  angles.reset(new angle_arr(size));

  mut(rgeTth) = Range();
  mut(rgeGma) = Range();
  mut(rgeGmaFull) = Range();

  EXPECT (size.i > cut.left + cut.right)
  EXPECT (size.j > cut.top  + cut.bottom)

  auto countWithoutCut = (size.i - cut.left - cut.right)
                       * (size.j - cut.top  - cut.bottom);
  EXPECT (countWithoutCut > 0)

  gmas.resize(countWithoutCut);
  gmaIndexes.resize(countWithoutCut);

  real d_midTth   = midTth.toRad(),
       cos_midTth = cos(d_midTth), sin_midTth = sin(d_midTth);

  real& d_z = detDist;
  real b_x1 = d_z * sin_midTth;
  real b_z1 = d_z * cos_midTth;

  for_int (i, size.i) {
    real d_x = (c::to_i(i) - midPix.i) * pixSize;

    real b_x = b_x1 + d_x * cos_midTth;
    real b_z = b_z1 - d_x * sin_midTth;

    real b_x2 = b_x*b_x;

    for_int (j, size.j) {
      real b_y = (midPix.j - c::to_i(j)) * pixSize; // == d_y
      real b_r = sqrt(b_x2 + b_y*b_y);

      c::rad gma = atan2(b_y, b_x);
      c::rad tth = atan2(b_r, b_z);

      mut(*angles).setAt(i, j, Angles(tth.toDeg(), gma.toDeg()));
    }
  }

  uint gi = 0;

  for (uint i = cut.left, iEnd = c::to_uint(size.i - cut.right); i < iEnd; ++i) {
    for (uint j = cut.top, jEnd = c::to_uint(size.j - cut.bottom); j < jEnd; ++j) {
      auto& as = angles->at(i, j);

      gmas.setAt(gi, as.gma);
      gmaIndexes.setAt(gi, i + j * c::to_uint(size.i));
      ++gi;

      mut(rgeTth).extendBy(as.tth);
      mut(rgeGmaFull).extendBy(as.gma);
      if (as.tth >= midTth)
        mut(rgeGma).extendBy(as.gma); // gma range at mid tth
    }
  }

  uint_vec is(countWithoutCut);
  for_i (c::to_uint(is.size()))
    is.setAt(i, i);

  std::sort(is.begin(), is.end(), [this](uint i1,uint i2) {
    real gma1 = gmas.at(i1), gma2 = gmas.at(i2);
    return gma1 < gma2;
  });

  c::vec<gma_t> gv(countWithoutCut);
  for_i (countWithoutCut)
    gv.setAt(i, gmas.at(is.at(i)));
  gmas = gv;

  uint_vec uv(countWithoutCut);
  for_i (countWithoutCut)
    uv.setAt(i, gmaIndexes.at(is.at(i)));
  gmaIndexes = uv;
}

//------------------------------------------------------------------------------
}
// eof
