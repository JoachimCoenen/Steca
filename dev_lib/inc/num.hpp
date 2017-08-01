// (dev_lib)

#pragma once

#include "../defs.hpp"

namespace l {
//------------------------------------------------------------------------------

// a number type alias; checked in debug mode
#ifndef NDEBUG
  #define use_num_(T, Base, ...) struct T {       \
    explicit T(Base val_) : val(val_) __VA_ARGS__ \
    operator Base() const VAL_(val)               \
  protected:                                      \
    Base val;                                     \
  };
#else
  #define use_num_(T, Base, ...) using T = Base;
#endif

use_num_(pint,  int,   { EXPECT_(0 <  val) }) // (p)ositive (int)eger
use_num_(peal,  real,  { EXPECT_(0 <  val) }) // (p)ositive r(eal)
use_num_(neal,  real,  { EXPECT_(0 <= val) }) // (n)on-negative r(eal)

//------------------------------------------------------------------------------
// integral type traits

template <typename T> struct __num_trait {};

// num type, is signed?, signed counterpart, unsigned counterpart
#define NUM_TYP_STRUCT_(num, issig, sig, uns)       \
  struct num##_t {                                  \
    typedef num typ; static typ const min, max;     \
    constexpr static bool const is_signed  = issig; \
    typedef sig i_typ;                              \
    typedef uns u_typ;                              \
  private:                                          \
    num##_t() = delete;                             \
  };                                                \
  template <> struct __num_trait<num> : num##_t {};

NUM_TYP_STRUCT_(uint8,  false, int8,  uint8)
NUM_TYP_STRUCT_( int8,  true,  int8,  uint8)
NUM_TYP_STRUCT_(uint16, false, int16, uint16)
NUM_TYP_STRUCT_( int16, true,  int16, uint16)
NUM_TYP_STRUCT_(uint32, false, int32, uint32)
NUM_TYP_STRUCT_( int32, true,  int32, uint32)
NUM_TYP_STRUCT_(uint64, false, int64, uint64)
NUM_TYP_STRUCT_( int64, true,  int64, uint64)

NUM_TYP_STRUCT_( flt32, true,  flt32, flt32)
NUM_TYP_STRUCT_( flt64, true,  flt64, neal)

#undef NUM_TYP_STRUCT_

#ifdef _MSC_VER // does not handle templates well

template <typename T, typename F>
inline T to_num(F f) {
  return T(f);
}

template <typename U>
inline typename __num_trait<U>::i_typ to_i(U u) {
  return to_num<typename __num_trait<U>::i_typ>(u);
}

template <typename I>
inline typename __num_trait<I>::u_typ to_u(I i) {
  return to_num<typename __num_trait<I>::u_typ>(i);
}

#else // _MSC_VER

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-parameter"

// casts to<-from: s_s, s_u, u_s, u_u

template <typename T>
struct __check__num__range {
  template <typename F> inline static void s_s(F f) {
    EXPECT_(__num_trait<T>::is_signed && __num_trait<F>::is_signed)
    EXPECT_(__num_trait<typename __num_trait<T>::typ>::min <= f &&
            f <= __num_trait<typename __num_trait<T>::typ>::max)
  }

  template <typename F> inline static void s_u(F f) {
    EXPECT_(__num_trait<T>::is_signed && !__num_trait<F>::is_signed)
    EXPECT_(f <= static_cast<typename __num_trait<T>::u_typ>(__num_trait<typename __num_trait<T>::typ>::max))
  }

  template <typename F> inline static void u_s(F f) {
    EXPECT_(!__num_trait<T>::is_signed && __num_trait<F>::is_signed)
    EXPECT_(0 <= f &&
            static_cast<typename __num_trait<F>::u_typ>(f) <= __num_trait<typename __num_trait<T>::typ>::max)
  }

  template <typename F> inline static void u_u(F f) {
    EXPECT_(!__num_trait<T>::is_signed && !__num_trait<F>::is_signed)
    EXPECT_(f <= __num_trait<typename __num_trait<T>::typ>::max)
  }
};

#pragma GCC diagnostic pop

// a cast between two number types
template <typename T, typename F>
inline T to_num(F f) {
  if (__num_trait<T>::is_signed) {
    if (__num_trait<F>::is_signed)
      __check__num__range<T>::s_s(f);
    else
      __check__num__range<T>::s_u(f);
  } else {
    if (__num_trait<F>::is_signed)
      __check__num__range<T>::u_s(f);
    else
      __check__num__range<T>::u_u(f);
  }

  return T(f);
}

// cast signed <- unsigned
template <typename U>
inline typename __num_trait<U>::i_typ to_i(U u) {
  static_assert(!__num_trait<U>::is_signed, "already signed");
  return to_num<typename __num_trait<U>::i_typ>(u);
}

// cast unsigned <- signed
template <typename I>
inline typename __num_trait<I>::u_typ to_u(I i) {
  static_assert(__num_trait<I>::is_signed, "already unsigned");
  return to_num<typename __num_trait<I>::u_typ>(i);
}

#endif

// cast to int
template <typename N>
inline int to_int(N n) {
  return to_num<int>(n);
}

// cast to uint
template <typename N>
inline uint to_uint(N n) {
  return to_num<uint>(n);
}

// cast to sz_t
template <typename N>
inline sz_t to_sz(N n) {
  return to_uint(n);
}

//------------------------------------------------------------------------------

extern real const flt_nan; // quiet nan
extern real const flt_inf;

bool isnan(flt32);
bool isfin(flt32);
bool isinf(flt32);

bool isnan(flt64);
bool isfin(flt64);
bool isinf(flt64);

bool isnormal(flt32);
bool isnormal(flt64);

int64 floor(flt32);
int64 floor(flt64);

int64 ceil(flt32);
int64 ceil(flt64);

int64 round(flt32);
int64 round(flt64);

template <typename T> T abs(T const& v) {
  return (0 <= v) ? v : -v;
}

template <typename T> T const& min(T const& a, T const& b) {
  return (a < b) ? a : b;
}

template <typename T> T const& max(T const& a, T const& b) {
  return (a > b) ? a : b;
}

template <typename T> T const& bound(T const& minVal, T const& v, T const& maxVal) {
  return max(minVal, min(v, maxVal));
}

// for flt32, flt64
template <typename T> T const& notnan(T const& a, T const& b) {
  EXPECT_(!isnan(b))
  return !isnan(a) ? a : b;
}

real pow(real x, uint n);

//------------------------------------------------------------------------------
}
// eof
