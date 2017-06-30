// (dev_lib)

#ifndef DEV_LIB_THREAD_HPP
#define DEV_LIB_THREAD_HPP

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

  template<typename L>
  mth_mut_(void, distribute, (L l, sz_t n)) {
    sz_t m = n / nc, mr = n % nc;
    for (sz_t i=0, from=0, to; i<nc; ++i, from=to)
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
#endif
// eof
