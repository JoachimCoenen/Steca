// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "sz2.hpp"
#include "vec.hpp"

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_(arr2)
  atr_(sz2, sz);
  atr_(vec<vec<T>>, ts);

  arr2(sz2 sz_, T val = T()) : sz(sz_), ts(sz.i, vec<T>(sz.j, val)) {}

  bol_(isEmpty, ()) RET_(sz.isEmpty())

  mth_(T const&, at, (uint i, uint j))  RET_(ts.at(i).at(j))
  mth_(T const&, at, (uint i))          RET_(at(i%sz.i, i/sz.j))

  mth_mut_(T&, refAt, (uint i, uint j)) RET_(mut(at(i,j)))
  mth_mut_(T&, refAt, (uint i))         RET_(mut(at(i)))

  set_(setAt, (uint i, uint j, T&& t))      SET_(refAt(i, j) = t)
  set_(setAt, (uint i, uint j, T const& t)) SET_(refAt(i, j) = t)
  set_(setAt, (uint i,         T&& t))      SET_(refAt(i)    = t)
  set_(setAt, (uint i,         T const& t)) SET_(refAt(i)    = t)
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
