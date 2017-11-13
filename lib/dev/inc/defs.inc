// (qt_lib)
// common definitions, include in a *source* file

/*
Include this file, directly or indirectly, into each header file in your project.
*/

#ifdef DEV_LIB_DEFS_INC
  #error inlude only once
#endif
#define DEV_LIB_DEFS_INC

//------------------------------------------------------------------------------
// struct implementation helpers

// comparable
#define COMPARABLE_IMPL(s) \
  int s::compare(rc that) const

#define RET_COMPARE_IF_NE(v1,v2) \
  if (v1 < v2) return -1;        \
  if (v1 > v2) return +1;

#define RET_BOOL_IF_LT_GT(v1,v2) \
  if (v1 < v2) return true;      \
  if (v1 > v2) return false;

#define RET_COMPARE_IF_NE_THAT(v) \
  RET_COMPARE_IF_NE(v, that.v)

#define RET_COMPARABLES_IF_NE(o)   \
  for (int cmp = o.compare(that.o); cmp; ) \
    return cmp;

// (in)equality ops
#define EQ_NE_IMPL(T) \
bool T::operator==(rc that) const RET_(0 == compare(that)) \
bool T::operator!=(rc that) const RET_(0 != compare(that))

#define LGTE_IMPL(T) \
bool T::operator< (rc that) const RET_(0 >  compare(that)) \
bool T::operator<=(rc that) const RET_(0 >= compare(that)) \
bool T::operator> (rc that) const RET_(0 <  compare(that)) \
bool T::operator>=(rc that) const RET_(0 <= compare(that))

#define COMP_OPS_IMPL(T) EQ_NE_IMPL(T) LGTE_IMPL(T)

// (in)equality ops for types that may be undefined
#define DEF_EQ_NE_IMPL(T) \
bool T::operator==(rc that) const RET_(isDef() && that.isDef() && 0 == compare(that)) \
bool T::operator!=(rc that) const RET_(isDef() && that.isDef() && 0 != compare(that))

#define DEF_LGTE_IMPL(T) \
bool T::operator< (rc that) const RET_(isDef() && that.isDef() && 0 >  compare(that)) \
bool T::operator<=(rc that) const RET_(isDef() && that.isDef() && 0 >= compare(that)) \
bool T::operator> (rc that) const RET_(isDef() && that.isDef() && 0 <  compare(that)) \
bool T::operator>=(rc that) const RET_(isDef() && that.isDef() && 0 <= compare(that))

#define DEF_COMP_OPS_IMPL(T) DEF_EQ_NE_IMPL(T) DEF_LGTE_IMPL(T)

//------------------------------------------------------------------------------
// debug support

#include <iostream>

#ifndef NDEBUG

struct _DebugOut_ {
  _DebugOut_() { std::cout << "[";              }
 ~_DebugOut_() { std::cout << "]" << std::endl; }

  template <typename T>
  _DebugOut_& operator<<(T const& t) {
    if (!first)
      std::cout << " :";
    first = false;
    std::cout << t;
    return *this;
  }

private:
  bool first = true;
};

// TRace
#define TR(what) { _DebugOut_() << what; }

#define EXPECT_TR_(cond, what) { if (!(cond)) TR(what); EXPECT_(cond) }
#define ENSURE_TR_(cond, what) { if (!(cond)) TR(what); ENSURE_(cond) }

#else

#define TR(what) {}

#define EXPECT_TR_(cond, what) { EXPECT_(cond) }
#define ENSURE_TR_(cond, what) { ENSURE_(cond) }

#endif

// WaTch
#define WT(what) TR(#what ">" << what)
// do not go here
#define NEVER_RETURN(what) EXPECT_(false) return what;

//------------------------------------------------------------------------------
// loops

#ifdef _MSC_VER // msvc fails templates

#define for_var_(i, n)   \
  for (int i = 0, i##End = (n); i < i##End; ++i)
#define for_var_down_(i, n) \
  for (int i = (n); i-- > 0; )

#else

#define for_var_(i, n)   \
  for (mut_typ<decltype(n)>::typ i = decltype(n)(0), i##End = (n); i < i##End; ++i)
#define for_var_down_(i, n) \
  for (mut_typ<decltype(n)>::typ i = (n); i-- > decltype(n)(0); )

#endif

#define for_i_(n)       for_var_(i, n)
#define for_i_down_(n)  for_var_down_(i, n)
#define for_ij_(ni, nj) for_var_(i, ni) for_var_(j, nj)

//------------------------------------------------------------------------------
// helpers

#define ONLY_ONCE \
{ static bool once = false; if (once) return; once = true; }


//------------------------------------------------------------------------------
// ad-hoc mutability

#include <utility>
template <typename T> void mut_swap(T& t1, T& t2) {
  std::swap(mut(t1), mut(t2));
}

//------------------------------------------------------------------------------
// includes

#include "exc.hpp"
#include "flt.hpp"
#include "num.hpp"
#include "ptr.hpp"
#include "str.hpp"
#include "str_cat.hpp"
#include "vecs.hpp"
#include <cmath>
#include <cctype>

//------------------------------------------------------------------------------
// testing

#include "../test/test.inc"

//------------------------------------------------------------------------------
// eof
