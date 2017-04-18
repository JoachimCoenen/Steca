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

#ifndef CORE_CURVE_HPP
#define CORE_CURVE_HPP

#include "def.hpp"
#include "fun.hpp"
#include "range.hpp"

namespace core {
//------------------------------------------------------------------------------
// A set of x-y datapoints.

dcl_struct_(Curve)
  atr_(real_vec, xs)
  atr_(real_vec, ys)
  atr_(Range, rgeX)
  atr_(Range, rgeY)

  Curve();

  void clear();

  bol_(isEmpty, ())
  mth_(uint, size, ())
  bol_(isSorted, ())

  set_(add, (real x, real y))

  mth_(Curve, intersect, (Range::rc))
  mth_(Curve, intersect, (Ranges::rc))

  voi_mut_(subtract, (Fun::rc))

  mth_(uint, maxYindex, ())  // the index of the maximum y value
  mth_(real, sumY, ())
};

using curve_vec =  c::vec<Curve>;

//------------------------------------------------------------------------------
}
#endif
// eof
