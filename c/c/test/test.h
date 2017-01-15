// c

#ifndef C_C_TEST_H
#define C_C_TEST_H

#ifdef WITH_TESTS
//------------------------------------------------------------------------------

#include "doctest.h"

#if defined(__GNUC__) && !defined(__APPLE__)
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#endif

#define TEST_CODE(...)  __VA_ARGS__
#define TEST(name, ...)                 \
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

#else

#define TEST_CODE(...)
#define TEST(name, ...)

#endif

//------------------------------------------------------------------------------
#endif
// eof
