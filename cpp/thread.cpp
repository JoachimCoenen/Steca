// cpp

#include "thread.h"
#include <c/cpp>
#include <vector>
#include "c/test/test.h"

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

#ifdef WITH_TESTS

static void f(uint dst[], uint const src[], size_t from, size_t to) {
  for (auto i=from; i<to; ++i)
    dst[i] = src[i]*src[i];
}

#endif

TEST("thread", ({
  uint const n = 10000;
  uint dst[n], src[n];
  for_i(n) src[i] = i;

  auto l = [&dst, &src](size_t from, size_t to) {
    for (auto i=from; i<to; ++i)
      dst[i] = src[i]*src[i];
  };

  threads ts;
  ts.split(l, n);

  for_i(n) src[i] = dst[i];

  ts.split(l, n);

  bool ok = true;
  for_i(n)
    if (dst[i] != i*i*i*i)
      ok = false;

  CHECK(ok);
});





)

//------------------------------------------------------------------------------
}
// eof
