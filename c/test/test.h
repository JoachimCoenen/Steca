// c

#ifndef C_C_TEST_H
#define C_C_TEST_H

#ifdef WITH_TESTS
//------------------------------------------------------------------------------

#include "doctest.h"

#if defined(__GNUC__) && !defined(__APPLE__)
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"
#endif

#define TEST(name, code) \
namespace {              \
TEST_CASE(name) {        \
  code                   \
}                        \
}

#else

#define TEST(name, code)

#endif

//------------------------------------------------------------------------------
#endif
// eof
