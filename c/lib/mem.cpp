// c

#include "test/test.h"

#include "mem.h"
#include "../def/cpp"
#include <memory>
#include <utility>

namespace unsafe {
  void malloc(void* p, size_t size) {
    p = ::malloc(size); // TODO abrt if null
  }

  void free(void* p) {
    if (p)
      ::free(p);
  }
}

C_CON_IMPL(mem, (size_t size_, void* p_)) : size(size_), p(p_) {}

namespace c {
//------------------------------------------------------------------------------

mem::mem(size_t size_) : base(size_, 0) {
  unsafe::malloc(p, size);
}

mem::mem(rval that) : base(0, nullptr) {
  std::swap(size, that.size);
  std::swap(p,    that.p);
}

mem::~mem() {
  unsafe::free(p);
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
