// (dev_lib)

#include "num.hpp"
#include "defs.inc"
#include <limits>

namespace l {
//------------------------------------------------------------------------------

#define _DEFINE_NUM_MIN_MAX_(T) \
T##_t::typ const T##_t::min = std::numeric_limits<T##_t::typ>::min(); \
T##_t::typ const T##_t::max = std::numeric_limits<T##_t::typ>::max();

_DEFINE_NUM_MIN_MAX_(uint8)
_DEFINE_NUM_MIN_MAX_(int8)
_DEFINE_NUM_MIN_MAX_(uint16)
_DEFINE_NUM_MIN_MAX_(int16)
_DEFINE_NUM_MIN_MAX_(uint32)
_DEFINE_NUM_MIN_MAX_(int32)
_DEFINE_NUM_MIN_MAX_(uint64)
_DEFINE_NUM_MIN_MAX_(int64)

//------------------------------------------------------------------------------

static_assert(
  1 == sizeof(int8) &&  1 == sizeof(uint8)  &&
  2 == sizeof(int16) && 2 == sizeof(uint16) &&
  4 == sizeof(int32) && 4 == sizeof(uint32) &&
  8 == sizeof(int64) && 8 == sizeof(uint64), "bad num size");

static_assert(
  4 == sizeof(flt32) && 8 == sizeof(flt64), "bad flt size");

//------------------------------------------------------------------------------

flt32 const flt32_nan = std::numeric_limits<flt32>::quiet_NaN();
flt32 const flt32_inf = std::numeric_limits<flt32>::infinity();

flt64 const flt64_nan = std::numeric_limits<flt64>::quiet_NaN();
flt64 const flt64_inf = std::numeric_limits<flt64>::infinity();

real const flt_nan = std::numeric_limits<real>::quiet_NaN();
real const flt_inf = std::numeric_limits<real>::infinity();

bool isnan(flt32 f) {
  return std::isnan(f);
}

bool isinf(flt32 f) {
  return std::isinf(f);
}

bool isfin(flt32 f) {
  return std::isfinite(f);
}

bool isnan(flt64 f) {
  return std::isnan(f);
}

bool isinf(flt64 f) {
  return std::isinf(f);
}

bool isfin(flt64 f) {
  return std::isfinite(f);
}

bool isnormal(flt32 f) {
  return std::isnormal(f);
}

bool isnormal(flt64 f) {
  return std::isnormal(f);
}

TEST_("num nan/fin/normal",
  float f;
  f = float(flt_nan);
    CHECK(isnan(f));  CHECK(!isfin(f)); CHECK(!isinf(f));
  f = float(flt_inf);
    CHECK(!isnan(f)); CHECK(!isfin(f)); CHECK(isinf(f));
  f = 1.f;
    CHECK(!isnan(f)); CHECK(isfin(f));  CHECK(!isinf(f));

  double d;
  d = double(float(flt_nan));
    CHECK(isnan(d));  CHECK(!isfin(d)); CHECK(!isinf(d));
  d = double(float(flt_inf)); CHECK(isinf(d));
    CHECK(!isnan(d)); CHECK(!isfin(d)); CHECK(isinf(d));
  d = 1.0;
    CHECK(!isnan(d)); CHECK(isfin(d));  CHECK(!isinf(d));

  CHECK(isnormal(1.f));  CHECK(isnormal(1.));
  CHECK(!isnormal(0.f)); CHECK(!isnormal(0.));

  CHECK(!isnormal(flt_inf)); CHECK(!isnormal(flt_nan));

  double subnormal = 1e-323;
  CHECK_NE(0.0, subnormal); CHECK(!isnormal(subnormal));
)

int64 floor(flt32 f) {
  return to_num<int64>(std::floor(f));
}

int64 floor(flt64 f) {
  return to_num<int64>(std::floor(f));
}

int64 ceil(flt32 f) {
  return to_num<int64>(std::ceil(f));
}

int64 ceil(flt64 f) {
  return to_num<int64>(std::ceil(f));
}

int64 round(flt32 f) {
  return to_num<int64>(std::round(f));
}

int64 round(flt64 f) {
  return to_num<int64>(std::round(f));
}

TEST_("num floor/ceil/...",
  CHECK_EQ(real(1), peal(1));
  CHECK_EQ(uint(1), pint(1));

  CHECK_EQ(0,  floor(0.1f));  CHECK_EQ(0,   floor(0.1));
  CHECK_EQ(-1, floor(-0.1f)); CHECK_EQ(-1,  floor(-0.1));

  CHECK_EQ(+1, ceil(0.1f));   CHECK_EQ(+1,  ceil(0.1));
  CHECK_EQ(0,  ceil(-0.1f));  CHECK_EQ(0,   ceil(-0.1));

  CHECK_EQ(1, min(1,2)); CHECK_EQ(1, min(3,2,1,4));
  CHECK_EQ(2, max(1,2));

  CHECK_EQ(1, bound(1,-3, 2)); CHECK_EQ(2, bound(1,+3, 2));

  CHECK_EQ(1, notnan(1., 2.)); CHECK_EQ(flt_inf, notnan(flt_inf, 2.));
  CHECK_EQ(2, notnan(flt_nan, 2.));
)

flt32 pow(flt32 x, uint n) {
  flt32 val = 1;
  while (n-- > 0)
    val *= x;
  return val;
}

flt64 pow(flt64 x, uint n) {
  flt64 val = 1;
  while (n-- > 0)
    val *= x;
  return val;
}

flt32 pow(flt32 x, flt32 y) {
  return std::pow(x, y);
}

flt64 pow(flt64 x, flt64 y) {
  return std::pow(x, y);
}

TEST_("num pow",
//  CHECK_EQ( 1, pow( 0,0));
//  CHECK_EQ( 1, pow( 1,0));
//  CHECK_EQ( 0, pow( 0,1));
//  CHECK_EQ( 1, pow(-1,0));
//  CHECK_EQ(-1, pow(-1,1));
//  CHECK_EQ(64, pow( 2,6));
)

//------------------------------------------------------------------------------
}
// eof
