// c

#include "mem.h"
#include "unsafe.h"
#include <c/c/cpp>

//------------------------------------------------------------------------------

_c_con_impl(mem, (size_t size_)) : size(size_), p(nullptr) {
  mut(p) = unsafe::mem::alloc(size);
}

_c_con_impl(mem, (size_t size_, pcvoid p_)) : size(size_), p(nullptr) {
  mut(p) = unsafe::mem::cpy(size, p_);
}

_c_des_impl(mem){
  unsafe::mem::free(zerop(p));
}

namespace c {
//------------------------------------------------------------------------------

mem::mem(size_t size_)            : c_mem(size_)     {}
mem::mem(size_t size, pcvoid src) : c_mem(size, src) {}

mem::mem(rval that) : c_mem(0) {
  unsafe::swap(size, that.size);
  unsafe::swap(p,    that.p);
}

#ifdef WITH_TESTS

mem mem_move() {
  return mem(99);
}

#endif

TEST("mem", ({
  mem m0(0), m1(1), mX(12345678);
  mem mm(mem_move());
});)

//------------------------------------------------------------------------------
}
// eof
