// (c)

#include "mem.h"
#include "../c_cpp"
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

pvoid maybe_alloc(sz_t sz) {
  return sz ? ::malloc(sz) : nullptr;
}

pvoid maybe_calloc(sz_t n, sz_t sz) {
  return sz*n ? ::calloc(n, sz) : nullptr;
}

pvoid maybe_realloc(pvoid p, sz_t sz) {
  return ::realloc(p, sz);
}

pvoid maybe_memcpy(sz_t sz, pcvoid src) {
  EXPECT(src || !sz)
  if (!src) return nullptr;
  pvoid p = maybe_alloc(sz);
  if (p) ::memcpy(p, src, sz);
  return p;
}

void free(pvoid p) {
  if (p) ::free(p);
}

pvoid alloc(sz_t sz) {
  auto p = maybe_alloc(sz);
  ABORT_IF(!p && sz, "alloc failed")
  return p;
}

pvoid calloc(sz_t n, sz_t sz) {
  auto p = maybe_calloc(n, sz);
  ABORT_IF(!p && sz*n, "calloc failed")
  return p;
}

pvoid realloc(pvoid p_, sz_t sz) {
  auto p = maybe_realloc(p_, sz);
  ABORT_IF(!p && sz, "realloc failed")
  return p;
}

pvoid memcpy(sz_t sz, pcvoid src) {
  auto p = maybe_memcpy(sz, src);
  ABORT_IF(!p && sz, "memcpy failed")
  return p;
}

TEST("unsafe",
  free(maybe_alloc(0));
  free(maybe_realloc(nullptr, 9));
)

//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
}
// eof
