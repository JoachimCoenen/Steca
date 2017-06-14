// (dev_lib)

#ifndef DEV_LIB_TYP_ARR2_H
#define DEV_LIB_TYP_ARR2_H

#include "../inc/defs_h.h"
#include "sz2.h"
#include "vec.h"

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_(arr2)
  atr_(sz2, sz);
  atr_(vec<vec<T>>, ts);

  arr2(sz2 sz_, T val = T()) : sz(sz_), ts(sz.i, vec<T>(sz.j, val)) {}

  bol_(isEmpty, ()) VAL_(sz.isEmpty())

  mth_(T const&, at, (sz_t i, sz_t j))  VAL_(ts.at(i).at(j))
  mth_(T const&, at, (sz_t i))          VAL_(at(i%sz.i, i/sz.j))

  mth_mut_(T&, refAt, (sz_t i, sz_t j)) VAL_(mut(at(i,j)))
  mth_mut_(T&, refAt, (sz_t i))         VAL_(mut(at(i)))

  set_(setAt, (sz_t i, sz_t j, T&& t))      SET_(refAt(i, j) = t)
  set_(setAt, (sz_t i, sz_t j, T const& t)) SET_(refAt(i, j) = t)
  set_(setAt, (sz_t i,         T&& t))      SET_(refAt(i)    = t)
  set_(setAt, (sz_t i,         T const& t)) SET_(refAt(i)    = t)
dcl_end

//------------------------------------------------------------------------------
}
#endif

// eof
