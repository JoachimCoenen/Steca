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
}

C_CON_IMPL(mem, (size_t size_, void* p_)) : size(size_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(size_t size_) : c_mem(size_, 0) {
  p = unsafe::malloc(size);
}

mem::mem(size_t size, void const* src) : mem(size) {
  unsafe::memcpy(p, src, size);
}

mem::mem(rval that) : c_mem(0, nullptr) {
  swap(that);
}

mem::~mem() {
  unsafe::free(p);
}

void mem::swap(ref that) {
  std::swap(size, that.size);
  std::swap(p,    that.p);
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
