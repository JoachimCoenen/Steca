// c

#ifndef C_CPP_ASYNC_H
#define C_CPP_ASYNC_H

#include <c/c/h>
#include <c/cpp/vec.h>
#include <thread>

namespace c {

_sub_struct (thread, std::thread) BASE_CONS
_sub_struct_end

struct threads {
  static uint num_cores();

  threads();

  template<typename L>
  void split(L l, sz_t n)
  {
    sz_t m = n / nc, mr = n % nc;

    for (sz_t i=0, from=0, to; i<nc; ++i, from=to)
      ts[i] = thread(l, from, (to = from + (i ? m : m+mr)));

    join();
  }

private:
  uint const     nc;
  c::vec<thread> ts;
  void join();
};

}

//------------------------------------------------------------------------------
#endif
// eof
