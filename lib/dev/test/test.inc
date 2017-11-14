// (dev_lib)

#ifdef DEV_LIB_TEST_INC
  #error inlude only once
#endif
#define DEV_LIB_TEST_INC

#ifdef WITH_TESTS
//------------------------------------------------------------------------------

#include "doctest.h"

#define TEST_CODE_(...)  __VA_ARGS__
#define TEST_(name, ...)                \
namespace {                             \
DOCTEST_TEST_CASE(name) { __VA_ARGS__ } \
}

#define CHECK           DOCTEST_CHECK
#define CHECK_FALSE     DOCTEST_CHECK_FALSE

#define CHECK_EQ        DOCTEST_CHECK_EQ
#define CHECK_NE        DOCTEST_CHECK_NE
#define CHECK_LT        DOCTEST_CHECK_LT
#define CHECK_LE        DOCTEST_CHECK_LE
#define CHECK_GT        DOCTEST_CHECK_GT
#define CHECK_GE        DOCTEST_CHECK_GE

#define CHECK_THROWS_AS DOCTEST_CHECK_THROWS_AS
#define CHECK_NOTHROW   DOCTEST_CHECK_NOTHROW

#else

#define TEST_CODE_(...)
#define TEST_(name, ...)

#endif

//------------------------------------------------------------------------------
// eof DOCS
