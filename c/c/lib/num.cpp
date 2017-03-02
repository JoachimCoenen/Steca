// (c)

#include "num.h"
#include "../../c/c_cpp"
#include <cmath>
#include <limits>

//------------------------------------------------------------------------------

#define C_DEFINE_NUM_MIN_MAX_(T) \
T##_t::typ const T##_t::min = std::numeric_limits<T##_t::typ>::min(); \
T##_t::typ const T##_t::max = std::numeric_limits<T##_t::typ>::max();

namespace c {
  C_DEFINE_NUM_MIN_MAX_(uint8)
  C_DEFINE_NUM_MIN_MAX_(int8)
  C_DEFINE_NUM_MIN_MAX_(uint16)
  C_DEFINE_NUM_MIN_MAX_(int16)
  C_DEFINE_NUM_MIN_MAX_(uint32)
  C_DEFINE_NUM_MIN_MAX_(int32)
  C_DEFINE_NUM_MIN_MAX_(uint64)
  C_DEFINE_NUM_MIN_MAX_(int64)
}

//------------------------------------------------------------------------------

static_assert(4 == sizeof(flt32) && 8 == sizeof(flt64), "bad fltXX size");

namespace c {
//------------------------------------------------------------------------------

#undef NAN
#undef INF

flt64 const NAN = std::numeric_limits<flt64>::quiet_NaN();
flt64 const INF = std::numeric_limits<flt64>::infinity();

bool isnan(flt32 f) {
  return std::isnan(f);
}

bool isinf(flt32 f) {
  return std::isinf(f);
}

bool isnan(flt64 f) {
  return std::isnan(f);
}

bool isinf(flt64 f) {
  return std::isinf(f);
}

bool isnormal(flt32 f) {
  return std::isnormal(f);
}

bool isnormal(flt64 f) {
  return std::isnormal(f);
}

TEST("num",
  float f;
  f = float(c::NAN); CHECK(isnan(f));
  f = float(INF);    CHECK(isinf(f));

  double d;
  d = double(float(c::NAN)); CHECK(isnan(d));
  d = double(float(INF));    CHECK(isinf(d));

  CHECK(isnormal(1.0));
  CHECK(isnormal(1.0f));
  CHECK(!isnormal(0.0));
  CHECK(!isnormal(INF));
  CHECK(!isnormal(NAN));
  double subnormal = 1e-323;
  CHECK_NE(0.0, subnormal);
  CHECK(!isnormal(subnormal));
)

TEST("p",
  CHECK_EQ(real(1), preal(1));
  CHECK_EQ(uint(1), pint(1));
)

//------------------------------------------------------------------------------
}
// eof
