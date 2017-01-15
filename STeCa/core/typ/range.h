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

#include <c/c/def/def_num>
#include <c/cpp/vec.hpp>

#define DATA_NS    core
#define DATA_NAME  Range

_c_data
  _var (real, min) _var (real, max) // this is the range
_c_data_end

_cpp_struct
Range();                          // undef'd (NaN)
Range(real val);                  // singular, min == max
Range(real min, real max);        // normal

Range(rc);

static Range infinite();          // -inf .. +inf

COMPARABLE EQ_NE

void  undef();                    // make NaNs
bool  isDef() const;              // is not NaNs
bool  empty() const;              // not def'd or empty

real  width()  const;
real  center() const;


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

protected:
ref operator=(rc);
_cpp_struct_end

#undef DATA_NS
#undef DATA_NAME

#define DATA_NS    core
#define DATA_NAME  Ranges

_c_data
_c_data_end

_cpp_struct
Ranges();
Ranges(rval);

void clear()             { rs.clear();        }
bool is_empty()    const { return rs.empty(); }
sz_t count()       const { return rs.size();  }
bool empty()       const { return rs.empty(); }

Range::rc at(sz_t i) const { return rs.at(i);       }

// collapses overlapping ranges into one; true if there was a change
bool add(Range::rc);

// removes (cuts out) a range; true if there was a change
bool rem(Range::rc);

private:
void sort();
c::vec<Range> rs;
_cpp_struct_end

#undef DATA_NS
#undef DATA_NAME

#endif
