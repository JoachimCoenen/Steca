// common definitions, include in a *source* file

#ifdef DEFS_CPP_H
  #error inlude only once
#endif
#define DEFS_CPP_H

#include "num.h"
#include <cmath>

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
bool T::operator==(rc that) const VAL_(0 == compare(that)) \
bool T::operator!=(rc that) const VAL_(0 != compare(that))

#define LGTE_IMPL(T) \
bool T::operator< (rc that) const VAL_(0 >  compare(that)) \
bool T::operator<=(rc that) const VAL_(0 >= compare(that)) \
bool T::operator> (rc that) const VAL_(0 <  compare(that)) \
bool T::operator>=(rc that) const VAL_(0 <= compare(that))

#define COMP_OPS_IMPL(T) EQ_NE_IMPL(T) LGTE_IMPL(T)

// (in)equality ops for types that may be undefined
#define DEF_EQ_NE_IMPL(T) \
bool T::operator==(rc that) const VAL_(isDef() && that.isDef() && 0 == compare(that)) \
bool T::operator!=(rc that) const VAL_(isDef() && that.isDef() && 0 != compare(that))

#define DEF_LGTE_IMPL(T) \
bool T::operator< (rc that) const VAL_(isDef() && that.isDef() && 0 >  compare(that)) \
bool T::operator<=(rc that) const VAL_(isDef() && that.isDef() && 0 >= compare(that)) \
bool T::operator> (rc that) const VAL_(isDef() && that.isDef() && 0 <  compare(that)) \
bool T::operator>=(rc that) const VAL_(isDef() && that.isDef() && 0 <= compare(that))

#define DEF_COMP_OPS_IMPL(T) DEF_EQ_NE_IMPL(T) DEF_LGTE_IMPL(T)

//------------------------------------------------------------------------------
// debug support

#ifndef NDEBUG

#include <iostream>

struct _DebugOut_ {
  _DebugOut_() { std::cout << "[";              }
 ~_DebugOut_() { std::cout << "]" << std::endl; }

  template<typename T>
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
#define TR_(what) { _DebugOut_() << what; }

#define EXPECT_TR_(cond, what) { if (!(cond)) TR(what); EXPECT_(cond) }
#define ENSURE_TR_(cond, what) { if (!(cond)) TR(what); ENSURE_(cond) }

#else

#define TR_(what) {}

#define EXPECT_TR_(cond, what) { EXPECT_(cond) }
#define ENSURE_TR_(cond, what) { ENSURE_(cond) }

#endif

// WaTch
#define WT_(what) TR_(#what ">" << what)
// do not go here
#define NEVER    EXPECT_(false)

//------------------------------------------------------------------------------
// loops

#ifdef _MSC_VER // msvc fails templates

#define for_var_(i, n)   \
  for (int i = 0, i##End = (n); i < i##End; ++i)
#define for_var_down_(i, n) \
  for (int i = (num); i-- > 0; )

#else

#define for_var_(i, n)   \
  for (mut_typ<decltype(n)>::typ i = decltype(n)(0), i##End = (n); i < i##End; ++i)
#define for_var_down_(i, n) \
  for (mut_typ<decltype(n)>::typ i = (num); i-- > decltype(n)(0); )

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
template<typename T> void mut_swap(T& t1, T& t2) {
  std::swap(mut(t1), mut(t2));
}

//------------------------------------------------------------------------------
// exceptions

#include "exc.h"

//------------------------------------------------------------------------------
// testing

#include "../test/test.h"

//------------------------------------------------------------------------------
// eof