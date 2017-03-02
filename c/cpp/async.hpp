// (c)

#ifndef C_CPP_ASYNC_HPP
#define C_CPP_ASYNC_HPP

#include "../../c/cpp/vec.hpp"
#include <thread>

#define NS__ c
#define ST__ thread

_struct_sub (std::thread) BASE_CONS
_struct_sub_end

#undef ST__
#undef NS__

namespace c {
//------------------------------------------------------------------------------

struct threads {
  static uint num_cores();

  threads();

  template<typename L>
  void distribute(L l, sz_t n) {
    sz_t m = n / nc, mr = n % nc;
    for (sz_t i=0, from=0, to; i<nc; ++i, from=to)
      ts.setAt(i, thread(l, from, (to = from + (i ? m : m+mr))));
    join();
  }

private:
  uint const     nc;
  c::vec<thread> ts;

  void join();
};

//------------------------------------------------------------------------------
}
#endif
// eof
