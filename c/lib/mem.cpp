// c

#include "mem.h"
#include "../def/def_cpp"
#include "../test/test.h"
#include <memory>
#include <string.h>
#include <utility>

namespace unsafe {
  void* malloc(size_t size) {
    return ::malloc(size); // TODO abrt if null
  }

  void free(void*& p) {
    if (p) {
      ::free(p); p = nullptr;
    }
  }

  void memcpy(void* dst, void const* src, size_t size) {
    ::memcpy(dst, src, size);
  }

  template<typename T>
  void swap(T& t1, T& t2) {
    std::swap(mut(t1), mut(t2));
  }
}

C_CON_C_IMPL(mem, (size_t size_, void* p_)) : size(size_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(size_t size_) : c_mem(size_, 0) {
  mut(p) = unsafe::malloc(size);
}

mem::mem(size_t size, void const* src) : mem(size) {
  unsafe::memcpy(mut(p), src, size);
}

mem::mem(rval that) : c_mem(0, nullptr) {
  swap(that);
}

mem::~mem() {
  unsafe::free(mut(p));
}

void mem::swap(ref that) {
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
