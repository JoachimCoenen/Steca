// cpp

#ifndef CPP_THREAD_H
#define CPP_THREAD_H

#include <c/h>
#include <thread>
#include <vector>

namespace c {

struct thread : std::thread { BASE(std::thread)
  using base::base;
};

struct threads {
  static uint num_cores();

  threads();

  template<typename L>
  void split(L l, size_t n)
  {
    size_t m = n / nc, mr = n % nc;

    for (size_t i=0, from=0, to; i<nc; ++i, from=to) {
      ts[i] = thread(l, from, (to = from + (i ? m : m+mr)));
    }

    join();
  }

private:
  uint const nc;
  std::vector<thread> ts;
  void join();
};

}

//------------------------------------------------------------------------------
#endif
// eof
