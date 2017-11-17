// (lib/dev)

/** @file
 * stronger-typed integral types; checked in debug mode;
 * simple aliases in release mode
 */

#pragma once

#include "../defs.hpp"

namespace l {
//------------------------------------------------------------------------------

#ifndef NDEBUG
  /** \def use_int_(T, Base, ...)
   * @c T is a struct wrapper around integral @c Base,
   * @c ... is a constructor code
   */
  #define use_int_(T, Base, ...)                  \
  struct T {                                      \
    explicit T(Base val_) : val(val_) __VA_ARGS__ \
    operator Base() const RET_(val)               \
    T& operator++()    SET_(++val)                \
    T& operator--()    SET_(--val)                \
    T  operator++(int) RET_(T(val++))             \
    T  operator--(int) RET_(T(val--))             \
  protected:                                      \
    Base val;                                     \
  };

  /** \def use_flt_(T, Base, ...)
   * @c T is a struct wrapper around floating-point @c Base,
   * @c ... is a constructor code
   */
  #define use_flt_(T, Base, ...)                  \
  struct T {                                      \
    explicit T(Base val_) : val(val_) __VA_ARGS__ \
    operator Base() const RET_(val)               \
  protected:                                      \
    Base val;                                     \
  };

#else

#define use_int_(T, Base, ...) using T = Base;
  #define use_flt_(T, Base, ...) using T = Base;

#endif

/// @c pint - a (p)ositive (int)eger
use_int_(pint,  uint,  { EXPECT_(0 <  val) })
/// @c peal - a (p)ositive r(eal)
use_flt_(peal,  real,  { EXPECT_(0 <  val) })
/// @c neal - a (n)on-negative r(eal)
use_flt_(neal,  real,  { EXPECT_(0 <= val) })

//------------------------------------------------------------------------------

/// integral number type trait: min / max / is_signed / signed and unsigned counterparts
template <typename T> struct __num_trait {};

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

#ifndef NDEBUG
NUM_TYP_STRUCT_( pint, false,  int,   pint)
NUM_TYP_STRUCT_( peal, false,  real,  peal)
NUM_TYP_STRUCT_( neal, false,  real,  neal)
#endif

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

/// checks if a number is in another type's range
template <typename T>
struct __check__num__range {
  /// signed from signed
  template <typename F> inline static void s_s(F f) {
    EXPECT_(__num_trait<T>::is_signed && __num_trait<F>::is_signed)
    EXPECT_(__num_trait<typename __num_trait<T>::typ>::min <= f &&
            f <= __num_trait<typename __num_trait<T>::typ>::max)
  }

  /// signed from unsigned
  template <typename F> inline static void s_u(F f) {
    EXPECT_(__num_trait<T>::is_signed && !__num_trait<F>::is_signed)
    EXPECT_(f <= static_cast<typename __num_trait<T>::u_typ>(__num_trait<typename __num_trait<T>::typ>::max))
  }

  /// unsigned from signed
  template <typename F> inline static void u_s(F f) {
    EXPECT_(!__num_trait<T>::is_signed && __num_trait<F>::is_signed)
    EXPECT_(0 <= f &&
            static_cast<typename __num_trait<F>::u_typ>(f) <= __num_trait<typename __num_trait<T>::typ>::max)
  }

  /// unsigned from unsigned
  template <typename F> inline static void u_u(F f) {
    EXPECT_(!__num_trait<T>::is_signed && !__num_trait<F>::is_signed)
    EXPECT_(f <= __num_trait<typename __num_trait<T>::typ>::max)
  }
};

#pragma GCC diagnostic pop

/// checked cast of a number to @c T from @c F
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

/// checked cast of a number to signed from unsigned (same bit width)
template <typename U>
inline typename __num_trait<U>::i_typ to_i(U u) {
  static_assert(!__num_trait<U>::is_signed, "already signed");
  return to_num<typename __num_trait<U>::i_typ>(u);
}

/// checked cast of a number to unsigned from signed (same bit width)
template <typename I>
inline typename __num_trait<I>::u_typ to_u(I i) {
  static_assert(__num_trait<I>::is_signed, "already unsigned");
  return to_num<typename __num_trait<I>::u_typ>(i);
}

#endif

/// checked cast of a number to @c int
template <typename N>
inline int to_int(N n) {
  return to_num<int>(n);
}

/// checked cast of a number to @c uint
template <typename N>
inline uint to_uint(N n) {
  return to_num<uint>(n);
}

/// get the minimum possible value
template <typename T>
inline T val_min(T const&) {
  return __num_trait<T>::min;
}

/// get the minimum possible value
template <typename T>
inline T val_max(T const&) {
  return __num_trait<T>::max;
}

//------------------------------------------------------------------------------

extern flt32 const flt32_nan; ///< quiet nan
extern flt32 const flt32_inf; ///< infinity

extern flt64 const flt64_nan; ///< quiet nan
extern flt64 const flt64_inf; ///< infinity

extern real  const real_nan;  ///< quiet nan
extern real  const real_inf;  ///< infinity

bool isnan(flt32);            ///< check if NaN
bool isinf(flt32);            ///< check if infinite
bool isfin(flt32);            ///< check if finite

bool isnan(flt64);            ///< check if NaN
bool isinf(flt64);            ///< check if infinite
bool isfin(flt64);            ///< check if finite

bool isnormal(flt32);         ///< check if normal
bool isnormal(flt64);         ///< check if normal

int64 floor(flt32);
int64 floor(flt64);

int64 ceil(flt32);
int64 ceil(flt64);

int64 round(flt32);
int64 round(flt64);

/// templated @c abs
template <typename T> T abs(T const& v) {
  return (0 <= v) ? v : -v;
}

/// templated @c min
template <typename T> T const& min(T const& a, T const& b) {
  return (a < b) ? a : b;
}

/// templated variadic @c min
template <typename T, typename... R> T const& min(T const& t, R const&... r) {
  return min(t, min(r...));
}

/// templated @c max
template <typename T> T const& max(T const& a, T const& b) {
  return (a > b) ? a : b;
}

/// templated variadic @c max
template <typename T, typename... R> T const& max(T const& t, R const&... r) {
  return max(t, max(r...));
}
/// templated @c bound (a value between min / max)
template <typename T> T const& bound(T const& minVal, T const& v, T const& maxVal) {
  return max(minVal, min(v, maxVal));
}

/// return @c a if it is not NaN, else @c b (must be NaN)
template <typename T> T const& notnan(T const& a, T const& b) {
  EXPECT_(!isnan(b))
  return !isnan(a) ? a : b;
}

flt32 pow(flt32 x, uint n);   ///< integral @c pow
flt64 pow(flt64 x, uint n);   ///< integral @c pow

flt32 pow(flt32 x, flt32 y);  ///< standard @c pow
flt64 pow(flt64 x, flt64 y);  ///< standard @c pow

//------------------------------------------------------------------------------
}
// eof
