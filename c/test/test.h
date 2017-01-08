// c

#ifndef C_TEST_H
#define C_TEST_H

#ifdef WITH_TESTS
//------------------------------------------------------------------------------

#include "doctest.h"

// TODO #pragma GCC diagnostic ignored "-Wgnu-statement-expression"

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
