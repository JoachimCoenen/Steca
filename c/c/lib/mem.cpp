// c

#include "mem.h"
#include "../cpp"
#include <memory>
#include <string.h>

c_mem::c_mem(sz_t sz_, pcvoid p_) : sz(sz_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(sz_t sz)
: c_base(sz, unsafe::calloc(1,sz)) {}

mem::mem(sz_t sz, pcvoid p)
: c_base(sz, unsafe::memcpy(sz, p)) {
  EXPECT(p || !sz)
}

mem::mem(rc that) : mem(that.sz, that.p) {}

mem::mem(rval that) : c_base(that.sz, nullptr) {
  mutate::swap(p, that.p);
}

mem::~mem() {
  unsafe::free(mut(p));
}

TEST_CODE(
  mem mem_move(mem&& m) { return std::move(m); }
)

TEST("mem",
  mem m0(0), m1(1), mX(12345678), mc(3, "Hello"), mcc(mc);
  mem mm(mem_move(mem(3, "  ")));
)

namespace unsafe {
//------------------------------------------------------------------------------

pvoid alloc(sz_t sz) {
  return (sz > 0) ? ::malloc(sz) : nullptr; // TODO abrt if null
}

pvoid calloc(sz_t n, sz_t sz) {
  return (sz > 0) ? ::calloc(n, sz) : nullptr; // TODO abrt if null
}

pvoid realloc(pvoid p, sz_t sz) {
  return ::realloc(p, sz); // TODO abrt if null
}

void free(pvoid p) {
  if (p) ::free(p);
}

pvoid memcpy(sz_t size, pcvoid src) {
  if (!src) return nullptr;
  pvoid p = alloc(size);
  ::memcpy(p, src, size);
  return p;
}

TEST("unsafe",
  free(alloc(0));
  free(realloc(nullptr, 9));
)

//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
}
// eof
