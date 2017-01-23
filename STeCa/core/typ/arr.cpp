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

#include "arr.hpp"
#include <c/c/cpp>
#include <c/c/lib/mem.h>
#include <c/c/lib/num.h>

core_sz2::core_sz2() : core_sz2(0, 0) {}
core_sz2::core_sz2(sz_t i_, sz_t j_) : i(i_), j(j_) {}

namespace core {
//------------------------------------------------------------------------------

COMPARABLE_IMPL(sz2) {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(sz2)

bool sz2::empty() const {
  return 0==i || 0==j; // 0 == size()
}

sz_t sz2::size() const {
  return i * j;
}

sz2 sz2::transposed() const {
  return sz2(j, i);
}

TEST("sz2",
  CHECK(sz2(0,8).empty());
  sz2 sz0, sz23(2,3), sz32(3,2);
  CHECK(sz0.empty());   CHECK_EQ(0, sz0.size());
  CHECK(!sz23.empty()); CHECK_EQ(6, sz23.size());
  CHECK(!sz32.empty()); CHECK_EQ(sz32.size(), sz23.size());
  CHECK_EQ(sz23, sz32.transposed());
)

sz2 operator-(sz2::rc s1, sz2::rc s2) {
  auto i = c::to_i(s1.i) - c::to_i(s2.i); if (i < 0) i = 0;
  auto j = c::to_i(s1.j) - c::to_i(s2.j); if (j < 0) j = 0;
  return sz2(c::to_u(i), c::to_u(j));
}

TEST("sz2-",
  sz2 sz = sz2(4,3) - sz2(3,4);
  CHECK_EQ(sz2(1,0), sz);
)

//------------------------------------------------------------------------------

arr2base::arr2base(sz2 sz_, sz_t tsize) : sz(sz_), ts(nullptr) {
  if (sz.empty())
    return;

  // allocate in smaller blocks (by columns) to avoid memory fragmentation
  mut(ts) = c::unsafe::calloc(sz.i, sizeof(void*));
  for_i (sz.i)
    static_cast<void**>(mut(ts))[i] = c::unsafe::calloc(sz.j, tsize);
}

arr2base::~arr2base() {
  if (ts) {
    for_i (sz.i)
      c::unsafe::free(static_cast<void**>(mut(ts))[i]);
    c::unsafe::free(static_cast<void*>(mut(ts)));
  }
}

void const* arr2base::colAt(sz_t i) const {
  EXPECT(i < sz.i)
  return static_cast<void const*const*>(ts)[i];
}

TEST("arr2",
  typedef int T;
  sz2 sz0, sz(20,30);
  arr2<T> a(sz), a0(sz0);
  CHECK_EQ(0, a.at(0,0));
  a.fill(1);
  CHECK_EQ(1, a.at(0,0));
  int c = 0;
  for_ij (sz.i, sz.j)
    a.setAt(i, j, c++);
  c = 0; bool ok = true;
  for_ij (sz.i, sz.j)
    if (c++ != a.at(i, j))
       ok = false;
  CHECK(ok);
)

//------------------------------------------------------------------------------
}
// eof
