// c

#include "mem.h"
#include "../cpp"
#include "unsafe.h"

c_mem::c_mem(sz_t sz_, pcvoid p_) : sz(sz_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(sz_t sz)
: c_base(sz, unsafe::alloc(sz)) {}

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
  mem rval_mem() { return mem(99); }
)

TEST("mem",
  mem m0(0), m1(1), mX(12345678), mc(3, "Hello");
  mem mm(rval_mem());
)

//------------------------------------------------------------------------------
}
// eof
