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

#include <c/c/h>
#include <c/cpp/vec.h>

namespace core {
//------------------------------------------------------------------------------
// a range of values - a closed interval

_struct (Range)
  Range();                          // undef'd (NaN)
  Range(real val);                  // singular, min == max
  Range(real min, real max);        // normal

  Range(Range const&);
  Range(Range&&);

  static Range infinite();          // -inf .. +inf

  COMPARABLE EQ_NE

  void  undef();                    // make NaNs
  bool  isDef() const;              // is not NaNs
  bool  empty() const;              // not def'd or empty

  real  width()  const;
  real  center() const;

  _var (real, min) _var (real, max) // this is the range

  void  set(rc);
  void  set(real val);              // make singular
  void  set(real min, real max);    // must be: min <= max
  void  safeSet(real, real);        // will be set in the right order min <= max

  static Range safeFrom(real, real); // safe factory

  void  extendBy(real);             // extend to include the number
  void  extendBy(rc);               // extend to include the range

  bool  contains(real)  const;
  bool  contains(rc)    const;
  bool  intersects(rc)  const;
  Range intersect(rc)   const;

  real bound(real)      const;      // limit the number to the interval
};

//------------------------------------------------------------------------------
// A set of *sorted* *non-overlapping* ranges

_struct (Ranges)
  Ranges();

  void   clear()             { rs.clear();            }
  bool   is_empty()    const { return rs.empty();  }
  sz_t   count()       const { return rs.size();     }

  Range::rc at(uint i) const { return rs.at(i);       }

  // collapses overlapping ranges into one; true if there was a change
  bool add(Range::rc);

  // removes (cuts out) a range; true if there was a change
  bool rem(Range::rc);

private:
  void sort();
  c::vec<Range> rs;
};

//------------------------------------------------------------------------------
}
#endif
