// (dev_lib)
// common definitions, include in a *header* file

#pragma once

// compiler-dependent diagnostic messages
#if defined(__clang__) || defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wswitch-enum"
  #pragma GCC diagnostic ignored "-Wpadded"
#endif

#ifdef __clang__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wreserved-id-macro"
#endif

// clang-special code guard
#ifdef __clang__
  #define _if_clang_(...) __VA_ARGS__
#else
  #define _if_clang_(...)
#endif

//------------------------------------------------------------------------------
// fabulous macros for declarations of structures, under these assumptions:
// default access to data: public, unless there is a reason to hide
//                         therefore no need to have getters
// default ro mode:        all is immutable, unless explicitely sated otherwise

// needed in macros with templates
#define COMMA ,

// declaration of self-referencing types, very useful:
// Self: this very type; rc: const reference, ref: reference; rval: r-value
#define _self_types_(s)     \
  using Self = s;           \
  using rc   = Self const&; \
  using ref  = Self&;       \
  using rval = Self&&;

// begin declaration of struct
#define dcl_(s) \
  struct s { _self_types_(s)

// begin declaration of polymorphic struct (the base type)
#define dcl_base_(s) \
  dcl_(s) virtual ~s();

// begin declaration of sub-struct
#define dcl_sub_(s, b) \
  struct s : b { _self_types_(s) using base = b; \
    base const& base_rc() const { return *this; } // access to base, instead of 'friend'

// begin a declaration of sub-struct from two bases
#define dcl_sub2_(s, b1, b2) \
  struct s : b1, b2 { _self_types_(s) using base = b2; \
    base const& base_rc() const { return *this; }

// begin declaration of a sub-struct that reimplements another:
// the base structure is hidden (protected, so subclasses also can access it)
// base_rc() allows deliberate access to base
#define dcl_reimpl_(s, b) \
  struct s : protected b { _self_types_(s) using base = b; \
    base const& base_rc() const { return *this; } // access to base, instead of 'friend'

// from two bases
#define dcl_reimpl2_(s, b1, b2) \
  struct s : b1, protected b2 { _self_types_(s) using base = b2; \
    base const& base_rc() const { return *this; } // access to base, instead of 'friend'

// end of declaration - either one of the above
#define dcl_end \
  };

// structure attributes (data members)
#define atr_(typ, name) typ const name          // immutable attribute
#define ptr_(typ, name) typ const * const name  // imm. pointer to imm. attr.
#define ref_(typ, name) typ const &       name  // immutable reference

// static constant
#define cst_(typ, name) static atr_(typ, name)

// factory
#define fry_(typ, mth, args) static typ mth args

// a method that does not mutate the state
#define mth_(typ, mth, args)              typ mth args const

// a method that mutates the state
#define mth_mut_(typ, mth, args)          typ mth args

// a predicate
#define bol_(mth, args)                   mth_(bool, mth, args)
#define bol_mut_(mth, args)               mth_mut_(bool, mth, args)

// conversion operator
#define cop_(op)                          operator op() const

// actions: void methods
#define act_(mth, args)                   mth_(void, mth, args)
#define act_mut_(mth, args)               mth_mut_(void, mth, args)

// setters: allow chaining; useful for reimplementation
#define set_(mth, args)                   mth_mut_(ref, mth, args)
#define RTHIS return *this;

// declare struct as comparable ...
#define COMPARABLE  int compare(rc) const;
// ... with equality ops
#define EQ_NE       bool operator==(rc) const; \
                    bool operator!=(rc) const;
// ... inequality ops
#define LGTE        bool operator< (rc) const; \
                    bool operator<=(rc) const; \
                    bool operator> (rc) const; \
                    bool operator>=(rc) const;
// ... equality and inequality ops
#define COMP_OPS    EQ_NE LGTE

// simple return
#define RET_(expr) { return expr; }

// simple return
#define SET_(...) { __VA_ARGS__; RTHIS }

//------------------------------------------------------------------------------
// request ad-hoc mutability

// make a type mutable
template <typename T> struct mut_typ            { using typ = T; };
template <typename T> struct mut_typ<T const>   { using typ = T; };

// make a pointer value mutable
template <typename T> T const*& mut(T const*const& t) \
  RET_(const_cast<T const*&>(t))
// make a value mutable
template <typename T> T& mut(T const& t) \
  RET_(const_cast<T&>(t))

// make a pointed-to value mutable
template <typename T> T* mutp(T const* t) \
  RET_(const_cast<T*>(t))

// take & null a pointer value
template <typename T> T const* take_p(T const*const& p) {
  auto _ = p; mut(p) = nullptr; return _;
}

//------------------------------------------------------------------------------
// primitive numeric types (there is a utility to having our own)

typedef unsigned int   uint;

typedef unsigned char  uint8;
typedef   signed char   int8;

typedef unsigned short uint16;
typedef   signed short  int16;

typedef unsigned int   uint32;
typedef   signed int    int32;

#if defined(_WIN64)
  typedef unsigned __int64 uint64;
  typedef   signed __int64  int64;
#elif defined(_WIN32)
  typedef unsigned long long uint64;
  typedef   signed long long  int64;
#else
  typedef unsigned long  uint64;
  typedef   signed long   int64;
#endif

// explicitely stated sizes of floats
typedef float  flt32;
typedef double flt64;

// a default float type
using real = flt64;

// make <math.h> behave the same on Windows
#ifdef _MSC_VER
  #define _USE_MATH_DEFINES
#endif

struct bol_vec;
struct int_vec;
struct uint_vec;
struct real_vec;

//------------------------------------------------------------------------------
// exception annotations

#define may_err  noexcept(false)
#define will_err noexcept(false)

//------------------------------------------------------------------------------
// signalling annotation

#define emits

//------------------------------------------------------------------------------
// strings

struct str;
struct str_vec;

using pcstr = char const*;
using strc  = str const&;

//------------------------------------------------------------------------------
// debug support

#include <assert.h>
#define EXPECT_(cond) assert(cond);
#define ENSURE_(cond) assert(cond);
#define NEEDED_(cond) (assert(cond), cond)

//------------------------------------------------------------------------------
// eof
