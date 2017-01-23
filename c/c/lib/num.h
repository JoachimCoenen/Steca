// c

#ifndef C_C_TYP_NUM
#define C_C_TYP_NUM

#include "../h"

EXTERN_C
//------------------------------------------------------------------------------

typedef unsigned char  uint8;
typedef   signed char   int8;

typedef unsigned short uint16;
typedef   signed short  int16;

typedef unsigned int   uint32;
typedef   signed int    int32;

#ifdef _WIN64
typedef unsigned __int64 uint64;
typedef   signed __int64  int64;
#else
typedef unsigned long  uint64;
typedef   signed long   int64;
#endif

//------------------------------------------------------------------------------
EXTERN_C_END

#if _is_cpp_
namespace c {
//------------------------------------------------------------------------------

template <typename T> struct __num_trait {};

#define NUM_TYP_STRUCT(plain, issig, sig, uns)      \
  struct plain##_t {                                \
    typedef plain typ; static typ const min, max;   \
    constexpr static bool const is_signed = issig;  \
    typedef sig i_typ;                              \
    typedef uns u_typ;                              \
  private:                                          \
    plain##_t() = delete;                           \
  };                                                \
  template <> struct __num_trait<plain> : plain##_t {};

NUM_TYP_STRUCT(uint8,  false, int8,  uint8)
NUM_TYP_STRUCT( int8,  true,  int8,  uint8)
NUM_TYP_STRUCT(uint16, false, int16, uint16)
NUM_TYP_STRUCT( int16, true,  int16, uint16)
NUM_TYP_STRUCT(uint32, false, int32, uint32)
NUM_TYP_STRUCT( int32, true,  int32, uint32)
NUM_TYP_STRUCT(uint64, false, int64, uint64)
NUM_TYP_STRUCT( int64, true,  int64, uint64)

#undef NUM_TYP_STRUCT

template <typename U>
inline typename __num_trait<U>::i_typ to_i(U u) {
  static_assert(!__num_trait<U>::is_signed, "already signed");
  EXPECT(u <= static_cast<typename __num_trait<U>::typ>(__num_trait<typename __num_trait<U>::i_typ>::max))
  return static_cast<typename __num_trait<U>::i_typ>(u);
}

template <typename I>
inline typename __num_trait<I>::u_typ to_u(I i) {
  static_assert(__num_trait<I>::is_signed, "already unsigned");
  EXPECT(0 <= i)
  return static_cast<typename __num_trait<I>::u_typ>(i);
}

//------------------------------------------------------------------------------
}
#endif

typedef float  flt32;
typedef double flt64;

typedef flt64  real;

#undef NAN
#undef INF

#if _is_cpp_
namespace c {
//------------------------------------------------------------------------------

extern flt64 const NAN; // silent nan
extern flt64 const INF;

bool isnan(flt32);
bool isinf(flt32);

bool isnan(flt64);
bool isinf(flt64);

bool isnormal(flt32);
bool isnormal(flt64);


template <typename T> T const& min(T const& a, T const& b) {
  return (a < b) ? a : b;
}

template <typename T> T const& max(T const& a, T const& b) {
  return (a > b) ? a : b;
}

template <typename T> T const& bound(T const& min, T const& v, T const& max) {
  return c::max(min, c::min(v, max));
}

#ifndef NDEBUG

struct pint {
  explicit pint(uint ui_) : ui(ui_) {
    EXPECT(0 < ui)
  }

  explicit pint(int i) : pint(to_u(i)) {
  }

  operator uint() const { return ui; }

protected:
  uint ui;
};

struct preal {
  explicit preal(real r_) : r(r_) {
    EXPECT(0 < r)
  }

  operator real() const { return r; }

protected:
  real r;
};

#else

typedef uint pint;
typedef real preal;

#endif

//------------------------------------------------------------------------------
}
#endif
#endif
// eof
