// c

#include "mem.h"
#include <c/c/cpp>
#include "unsafe.h"

c_mem::c_mem(sz_t sz_, pcvoid p_) : sz(sz_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(sz_t sz) : c_mem(sz, unsafe::mem::alloc(sz)) {}

mem::mem(sz_t sz, pcvoid p) : c_mem(sz, unsafe::mem::cpy(sz, p)) {}

mem::mem(rc that) : mem(that.sz, that.p) {}

mem::mem(rval that) : c_mem (that.sz, nullptr) {
  unsafe::swap(p, that.p);
}

mem::~mem() {
  unsafe::mem::free(mut(p));
}

TEST_CODE(
mem mem_move() {
  return mem(99);
}
)

TEST("mem",
  mem m0(0), m1(1), mX(12345678);
  mem mm(mem_move());
)

//------------------------------------------------------------------------------
}
// eof
