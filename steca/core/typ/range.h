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

#ifndef CORE_RANGE_H
#define CORE_RANGE_H

#include "def.h"

dcl_(core_Range)
  using rv_t = real;

  atr_(rv_t, min);
  atr_(rv_t, max);
  core_Range(rv_t, rv_t);
dcl_end

namespace core {

dcl_sub_(Range, core_Range) COMPARABLE EQ_NE
  Range();                          // undef'd (NaN)
  Range(rv_t);                      // singular, min == max
  Range(rv_t, rv_t);                // the usual

  Range(rc);

  static Range inf();                // -inf .. +inf

  bool  isDef()   const;             // is not NaNs
  bool  isEmpty() const;             // not def'd or empty

  rv_t  width()   const;
  rv_t  center()  const;

  static Range safeFrom(rv_t, rv_t); // safe factory

  void  extendBy(rv_t);             // extend to include the number
  void  extendBy(rc);               // extend to include the range

  bool  contains(rv_t)  const;
  bool  contains(rc)    const;
  bool  intersects(rc)  const;
  Range intersect(rc)   const;

  rv_t  bound(rv_t)     const;      // limit the number to the interval

  ref   operator=(rc);
dcl_end

dcl_(Ranges) EQ_NE
  Ranges();

  void clear()         { rs.clear();          }
  bool isEmpty() const VAL_(rs.isEmpty())
  sz_t size()    const VAL_(rs.size())

  Range::rc at(sz_t i) const VAL_(rs.at(i))

  // collapses overlapping ranges into one; true if there was a change
  bool add(Range::rc);

  // removes (cuts out) a range; true if there was a change
  bool rem(Range::rc);

private:
  void sort();
  l::vec<Range> rs;
dcl_end

}
#endif
// eof