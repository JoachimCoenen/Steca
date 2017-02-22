// (c)

#include "async.hpp"
#include "../c/cpp"

namespace c {
//------------------------------------------------------------------------------

uint c::threads::num_cores() {
  return thread::hardware_concurrency();
}

c::threads::threads() : nc(num_cores()), ts(nc) {
}

void c::threads::join() {
  for (thread& t: ts)
    t.join();
}

TEST("thread",
  uint const n = 10000;
  uint dst[n], src[n];
  for_i(n) src[i] = i;

  auto l = [&dst, &src](sz_t from, sz_t to) {
    for (auto i=from; i<to; ++i)
      dst[i] = src[i]*src[i];
  };

  threads ts;
  ts.distribute(l, n);

  for_i(n) src[i] = dst[i];

  ts.distribute(l, n);

  bool ok = true;
  for_i(n)
    if (dst[i] != i*i*i*i)
      ok = false;

  CHECK(ok);
)

//------------------------------------------------------------------------------
}
// eof
