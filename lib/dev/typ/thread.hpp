// (dev_lib)

#pragma once
#include "vec.hpp"
#include <thread>

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(thread, std::thread)
  using base::base;
dcl_end

dcl_(threads)
  static uint num_cores();

  threads();

  template <typename L>
  mut_(distribute, (L l, uint n)) {
    uint m = n / nc, mr = n % nc;
    for (uint i=0, from=0, to; i<nc; ++i, from=to)
      ts.setAt(i, thread(l, from, (to = from + (i ? m : m+mr))));
    join();
  }

private:
  uint const  nc;
  vec<thread> ts;

  void join();
dcl_end

//------------------------------------------------------------------------------
}
// eof
