// (c) - include into any .c or .cpp file
// The most important definitions that *everyone* needs.

#ifndef C_C_H
#define C_C_H

#include "c_is_cpp.inc"

#ifdef _MSC_VER
  #define _USE_MATH_DEFINES
#endif

#ifdef GENERATE_INTERFACE // to produce .i files; not to compile
  #define NDEBUG
#else
  #include <stddef.h>
  typedef size_t       sz_t;

  typedef unsigned int uint;

  #include <assert.h>
  #define EXPECT(cond) assert(cond);
  #define ENSURE(cond) assert(cond);
#endif

#if _is_cpp_
  #define EXTERN_C extern "C" {
  #define EXTERN_C_END }
#else
  #define EXTERN_C
  #define EXTERN_C_END
#endif

#endif
// eof
