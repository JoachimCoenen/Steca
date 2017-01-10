// c

#include "def_num"
#include "../test/test.h"
#include <cmath>
#include <limits>

//------------------------------------------------------------------------------

#define _DEFINE_NUM_MIN_MAX_(T) \
T##_t::typ const T##_t::min = std::numeric_limits<T##_t::typ>::min(); \
T##_t::typ const T##_t::max = std::numeric_limits<T##_t::typ>::max();

namespace c {
  _DEFINE_NUM_MIN_MAX_(uint8)
  _DEFINE_NUM_MIN_MAX_(int8)
  _DEFINE_NUM_MIN_MAX_(uint16)
  _DEFINE_NUM_MIN_MAX_(int16)
  _DEFINE_NUM_MIN_MAX_(uint32)
  _DEFINE_NUM_MIN_MAX_(int32)
  _DEFINE_NUM_MIN_MAX_(uint64)
  _DEFINE_NUM_MIN_MAX_(int64)
}

//------------------------------------------------------------------------------

static_assert(4 == sizeof(flt32) && 8 == sizeof(flt64), "bad fltXX size");

namespace c {
//------------------------------------------------------------------------------

#undef NAN
#undef INF

flt64 const NAN = std::numeric_limits<flt64>::quiet_NaN();
flt64 const INF = std::numeric_limits<flt64>::infinity();

TEST("NAN/INF", ({
  double d;
  d = float(NAN); CHECK(isnan(d));
  d = float(INF); CHECK(isinf(d));
  float f;
  f = float(NAN); CHECK(isnan(f));
  f = float(INF); CHECK(isinf(f));
});)

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

//------------------------------------------------------------------------------
}
// eof
