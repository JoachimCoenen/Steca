// (c)
// The most important definitions that *everyone* needs.
// Include everywhere (indirectly).

#ifndef C_C_H
#define C_C_H

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

#ifdef __cplusplus

#define _is_cpp_ (1)
#define EXTERN_C extern "C" {
#define EXTERN_C_END }

#else

#define _is_cpp_ (0)
#define EXTERN_C
#define EXTERN_C_END

#endif

#endif
// eof
