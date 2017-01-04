// c

#ifndef C_NUM_H
#define C_NUM_H

#include "h"

EXTERN_C
//------------------------------------------------------------------------------

typedef unsigned char  uint8;
typedef   signed char   int8;

typedef unsigned short uint16;
typedef   signed short  int16;

typedef unsigned int   uint32;
typedef   signed int    int32;

typedef unsigned long  uint64;
typedef   signed long   int64;

typedef unsigned int   uint;

//------------------------------------------------------------------------------
EXTERN_C_END

#if __cpp__
namespace c {
//------------------------------------------------------------------------------

struct uint8 {
  typedef ::uint8 typ;
  static typ const min, max;
};

struct int8 {
  typedef ::int8 typ;
  static typ const min, max;
};

struct uint16 {
  typedef ::uint16 typ;
  static typ const min, max;
};

struct int16 {
  typedef ::int16 typ;
  static typ const min, max;
};

struct uint32 {
  typedef ::uint32 typ;
  static typ const min, max;
};

struct int32 {
  typedef ::int32 typ;
  static typ const min, max;
};

struct uint64 {
  typedef ::uint64 typ;
  static typ const min, max;
};

struct int64 {
  typedef ::int64 typ;
  static typ const min, max;
};

//------------------------------------------------------------------------------
}
#endif

typedef float  flt32;
typedef double flt64;

typedef flt64  real;

#if __cpp__
namespace c {
//------------------------------------------------------------------------------

bool isnan(flt32);
bool isinf(flt32);

bool isnan(flt64);
bool isinf(flt64);

bool isnormal(flt32);
bool isnormal(flt64);

//------------------------------------------------------------------------------
}
#endif

//------------------------------------------------------------------------------
#endif
// eof
