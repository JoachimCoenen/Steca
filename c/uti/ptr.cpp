// c

#include "test/test.h"

#include "ptr.h"
#include "../def/cpp"
#include <atomic>

C_CON_IMPL(ptr, (void* p_)) : p(p_) {}

C_MTH_IMPL(ptr, void*, take, ()) {
  void* _ = p; p = nullptr; return _;
}

//------------------------------------------------------------------------------

namespace c {

struct o_cnt {
  void* o;
  std::atomic_ulong cnt;

  o_cnt(void* o_) : o(o_) {}
};

counted_base::counted_base(void* o) : c_ptr(nullptr) {
  c_ptr::p = new o_cnt(o);
  inc();
}

counted_base::counted_base(counted_base const& that) : c_ptr(that.c_ptr::p) {
  inc();
}

void* counted_base::p() const {
  return static_cast<o_cnt*>(c_ptr::p)->o;
}

void counted_base::inc() {
  ++(static_cast<o_cnt*>(c_ptr::p)->cnt);
}

bool counted_base::dec() {
  return 0 == --(static_cast<o_cnt*>(c_ptr::p)->cnt);
}

void counted_base::cleanup() {
  delete static_cast<o_cnt*>(c_ptr::p);
}

TEST("scoped", ({
  scoped<uint> s1(new uint(8)), s2(new uint(9));
  CHECK_EQ(8, *s1);
  delete s1.take();
});)

TEST("counted", ({
  counted<uint> s1(new uint(8)), s2(s1);
  CHECK_EQ(*s1, *s2);
  CHECK_EQ(8,   *s2);
});)

//------------------------------------------------------------------------------
}
// eof
