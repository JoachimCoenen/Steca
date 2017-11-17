/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

#pragma once

/** @file
 * A representation of a numeric range (a.k.a. interval)
 */

#include <lib/dev/typ/vec.hpp>

namespace core {
//------------------------------------------------------------------------------

/** A numeric range (interval). It can be undefined (NaN, NaN), or defined
 * (not-NaN, not-NaN). The lower endpoint of a defined interval can be finite or
 * -Inf, the higher endpoint either finite or +Inf.
 */
dcl_(Range) COMPARABLE EQNE
  using rv_t = real;          ///< (r)ange (v)alue

  atr_(rv_t, min);            ///< the lower endpoint
  atr_(rv_t, max);            ///< the higher endpoint

  Range();                    ///< undefined (NaN, NaN)
  Range(rv_t);                ///< singular - one point (counts as empty)
  Range(rv_t min, rv_t max);  ///< the usual

  Range(rc);

  static Range inf();         ///< an infinite range (-inf .. +inf)

  bool  isDef()   const;      ///< is defined (not NaN-NaN)
  bool  isEmpty() const;      ///< not defined or empty

  rv_t  width()   const;
  rv_t  center()  const;

  static Range safeFrom(rv_t, rv_t);  ///< orders the endpoints

  void  extendBy(rv_t);             // extend to include the number
  void  extendBy(rc);               // extend to include the range

  bool  contains(rv_t)  const;
  bool  contains(rc)    const;
  bool  intersects(rc)  const;
  Range intersect(rc)   const;

  rv_t  bound(rv_t)     const;      // limit the number to the interval

  ref   operator=(rc);
dcl_end

//------------------------------------------------------------------------------

dcl_reimpl_(Ranges, l::vec<Range>) EQNE
  UB7_(base, clear, isEmpty, size, at, begin, end)

  Ranges();

  // collapses overlapping ranges into one; true if there was a change
  bool add(Range::rc);

  // removes (cuts out) a range; true if there was a change
  bool rem(Range::rc);

private:
  void sort();
  l::vec<Range> rs;
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
