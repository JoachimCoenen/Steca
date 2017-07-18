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

#include "curve.hpp"
#include <dev_lib/defs.inc>
#include <algorithm>

namespace core {
//------------------------------------------------------------------------------

Curve::Curve() : xs(), ys() {}

Curve::ref Curve::operator=(rc that) {
  mut(xs)   = that.xs;
  mut(ys)   = that.ys;
  mut(rgeX) = that.rgeX;
  mut(rgeY) = that.rgeY;
  RTHIS
}

void Curve::clear() {
  mut(xs).clear();
  mut(ys).clear();
  mut(rgeX) = Range();
  mut(rgeY) = Range();
}

bool Curve::isEmpty() const {
  ENSURE_(xs.isEmpty() == ys.isEmpty())
  return xs.isEmpty();
}

uint Curve::size() const {
  ENSURE_(xs.size() == ys.size())
  return xs.size();
}

bool Curve::isSorted() const {
  return std::is_sorted(xs.cbegin(), xs.cend());
}

Curve::ref Curve::add(real x, real y) {
  mut(xs).add(x);
  mut(ys).add(y);
  mut(rgeX).extendBy(x);
  mut(rgeY).extendBy(y);
  return *this;
}

Curve Curve::intersect(Range::rc range) const {
  Curve res;

  if (!range.isEmpty()) {
    ENSURE_(isSorted())

    uint xi = 0; auto sz = size();
    auto minX = range.min, maxX = range.max;
    while (xi < sz && xs.at(xi) < minX)
      ++xi;
    while (xi < sz && xs.at(xi) <= maxX) {
      res.add(xs.at(xi), ys.at(xi));
      ++xi;
    }
  }

  return res;
}

Curve Curve::intersect(Ranges::rc ranges) const {
  Curve res;

  // collect points that are in ranges
  // it works because both curve points and ranges are ordered and ranges are
  // non-overlapping
  ENSURE_(isSorted())

  uint xi = 0; auto sz = size();
  for_i_(ranges.size()) {
    auto& range = ranges.at(i);
    auto  minX = range.min, maxX = range.max;
    while (xi < sz && xs.at(xi) < minX)
      ++xi;
    while (xi < sz && xs.at(xi) <= maxX) {
      res.add(xs.at(xi), ys.at(xi));
      ++xi;
    }
  }

  return res;
}

Curve::ref Curve::subtract(Fun::rc f) {
  for_i_(size())
    mut(ys).refAt(i) -= f.y(xs.at(i));
  return *this;
}

uint Curve::maxYindex() const {
  if (isEmpty())
    return 0;

  auto yMax  = ys.at(0);
  uint index = 0;

  for_i_(size()) {
    auto y = ys.at(i);
    if (y > yMax) {
      yMax  = y;
      index = i;
    }
  }

  return index;
}

real Curve::sumY() const {
  real sum = 0;
  for_i_(size())
    sum += ys.at(i);
  return sum;
}

//------------------------------------------------------------------------------
}
// eof
