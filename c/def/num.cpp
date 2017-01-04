// c

#include "num.h"
#include <cmath>
#include <limits>

//------------------------------------------------------------------------------

#define _DEFINE_NUM_MIN_MAX_(T) \
T::typ const T::min = std::numeric_limits<T::typ>::min(); \
T::typ const T::max = std::numeric_limits<T::typ>::max();

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
//------------------------------------------------------------------------------
// eof
