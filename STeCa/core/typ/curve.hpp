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

dcl_struct (Curve)
  _atr (real_vec, xs)
  _atr (real_vec, ys)
  _atr (Range, rgeX)
  _atr (Range, rgeY)

  Curve();

  void clear();

  _bol (isEmpty, ())
  _mth (uint, size, ())
  _bol (isSorted, ())

  _set(add, (real x, real y))

  _mth (Curve, intersect, (Range::rc))
  _mth (Curve, intersect, (Ranges::rc))

  _voi_mut (subtract, (Fun::rc))

  _mth (uint, maxYindex, ())  // the index of the maximum y value
  _mth (real, sumY, ())
};

typedef c::vec<Curve> curve_vec;

//------------------------------------------------------------------------------
}
#endif
// eof
