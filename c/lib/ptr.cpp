// c

#include "ptr.h"
#include "../cpp"
#include "../def/def_num"
#include <atomic>

_c_con_impl(ptr, (pcvoid p_)) : p(p_) {}

_c_mth_impl(ptr, pcvoid, take, ()) {
  auto _ = mut(p); mut(p) = nullptr; return _;
}

//------------------------------------------------------------------------------

namespace c {

TEST("just_ptr", ({
  int i = 0;
  auto p1 = just_ptr<int>::from(&i), p2(p1);
  CHECK_EQ(p1, p2);

  ++(mut(*p1));
  ++(mut(*p1.ptr()));
  CHECK_EQ(2, *p2);
});)

// object ptr + counter
struct o_cnt {
  pcvoid o;
  std::atomic_size_t cnt;

  o_cnt(pcvoid o_) : o(o_), cnt(0) {}

  static o_cnt const* from(pcvoid p) {
    return static_cast<o_cnt const*>(p);
  }
};

TEST("o_cnt", ({
  typedef uint t;
  scoped<t> n(new t);
  o_cnt oc(n);
  CHECK_EQ(n, oc.o);
  CHECK_EQ(0, oc.cnt);
});)

_shared_base_::_shared_base_(pcvoid o) : c_ptr(nullptr) {
  mut(c_ptr::p) = new o_cnt(o);
  inc();
}

_shared_base_::_shared_base_(_shared_base_ const& that) : c_ptr(that.c_ptr::p) {
  inc();
}

pcvoid _shared_base_::p() const {
  return static_cast<o_cnt const*>(c_ptr::p)->o;
}

void _shared_base_::inc() {
  ++(mut(static_cast<o_cnt const*>(c_ptr::p)->cnt));
}

bool _shared_base_::dec() {
  return 0 == --(mut(static_cast<o_cnt const*>(c_ptr::p)->cnt));
}

void _shared_base_::cleanup() {
  delete static_cast<o_cnt*>(mut(c_ptr::p));
}

#ifdef WITH_TESTS
struct __test_instance_cnt {
  static int cnt;
  __test_instance_cnt() { ++cnt; }
 ~__test_instance_cnt() { --cnt; }
};

int __test_instance_cnt::cnt = 0;
#endif

TEST("scoped", ({
  scoped<uint> s1(new uint(8)), s2(new uint(9));
  CHECK_EQ(8, *s1);
  CHECK(bool(s1));
  delete s1.take();
  CHECK_FALSE(bool(s1));

  typedef __test_instance_cnt cnt;
  typedef scoped<cnt> sco_ptr;
  {
    cnt *c;
    sco_ptr p((c = new cnt));
    CHECK_EQ(1, cnt::cnt);
    CHECK(bool(p));
    CHECK_EQ(c, p);
    CHECK_EQ(1, p->cnt);
  }

  CHECK_EQ(0, cnt::cnt);

  own_ptr<cnt> raw;

  {
    sco_ptr p(new cnt);
    CHECK(bool(p));
    CHECK_EQ(1, cnt::cnt);
    raw.set(p.take());
    CHECK_FALSE(bool(p));
    CHECK_EQ(1, cnt::cnt);
  }

  CHECK_EQ(1, cnt::cnt);

  {
    sco_ptr p(raw);
    CHECK(bool(p));
    CHECK_EQ(1, cnt::cnt);
  }

  CHECK_EQ(0, cnt::cnt);
});)


TEST("shared", ({
  typedef uint t;
  auto n = new t(8);
  shared<t> s1(n);
  shared<t> s2(s1);
  CHECK_EQ(*s1, *s2);
  CHECK_EQ(8,   *s2);
});)

//------------------------------------------------------------------------------
}
// eof
