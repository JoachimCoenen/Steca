// c

#ifndef C_CPP_ASYNC_HPP
#define C_CPP_ASYNC_HPP

#include <c/cpp/vec.hpp>
#include <thread>

#define DATA_NS   c
#define DATA_NAME thread

_struct_sub(std::thread)
  using base::base;
_struct_sub_end

#undef DATA_NS
#undef DATA_NAME

namespace c {
//------------------------------------------------------------------------------

struct threads {
  static uint num_cores();

  threads();

  template<typename L>
  void distribute(L l, sz_t n) {
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

//------------------------------------------------------------------------------
}
#endif
// eof
