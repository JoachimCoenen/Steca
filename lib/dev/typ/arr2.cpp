// (lib/dev)

#include "arr2.hpp"
#include "../inc/defs.inc"

namespace l {
//------------------------------------------------------------------------------

TEST_("arr2",
  using T = int;
  sz2 sz0, sz(20,30);
  arr2<T> a(sz), a0(sz0);
  CHECK_EQ(0, a.at(0,0));

  int c = 0;
  for_ij_(sz.i, sz.j)
    a.setAt(i, j, c++);
  c = 0; bool ok = true;
  for_ij_(sz.i, sz.j)
    if (c++ != a.at(i, j))
       ok = false;
  CHECK(ok);
)

//------------------------------------------------------------------------------
}
// eof
