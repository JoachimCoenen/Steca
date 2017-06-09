// common definitions, include in a *header* file

#ifndef DEV_LIB_DEFS_H_H
#define DEV_LIB_DEFS_H_H
//------------------------------------------------------------------------------

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
  struct s : b { _self_types_(s) using base = b;

// begin a declaration of sub-struct fom two bases
#define dcl_sub2_(s, b1, b2) \
  struct s : b1, b2 { _self_types_(s) using base = b2;

// begin declaration of a sub-struct that reimplements another:
// the base structure is hidden (private)
// base_rc() allows deliberate access to base
#define dcl_reimpl_(s, b) \
  struct s : private b { _self_types_(s) using base = b;

// end of declaration - either one of the above
#define dcl_end \
  };

// structure attributes (data members)
#define atr_(typ, name) typ const name          // immutable attribute
#define mut_(typ, name) typ name                // mutable attribute
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

// conversion operator
#define cop_(op)                          operator op const

// setters: allow chaining; useful for reimplementation
#define set_(mth, args)                   mth_mut_(ref, mth, args)
#define RT return *this;

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

//------------------------------------------------------------------------------
// request ad-hoc mutability

// make a type mutable
template <typename T> struct mut_typ            { using typ = T; };
template <typename T> struct mut_typ<T const>   { using typ = T; };

// make a pointer value mutable
template <typename T> T const*& mut(T const*const& t) \
  { return const_cast<T const*&>(t); }
// make a value mutable
template <typename T> T& mut(T const& t) \
  { return const_cast<T&>(t); }

// make a pointed-to value mutable
template <typename T> T* mutp(T const* t) \
  { return const_cast<T*>(t); }

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

// our own (small) size type
typedef uint32 sz_t;

// explicitely stated sizes of floats
typedef float  flt32;
typedef double flt64;

// a default float type
using real = flt64;

// make <math.h> behave the same on Windows
#ifdef _MSC_VER
  #define _USE_MATH_DEFINES
#endif

//------------------------------------------------------------------------------
// strings

using pcstr = char const*;

#include <string>

using str  = std::string;
using strc = str const&;

extern str const nullstr;

//------------------------------------------------------------------------------
// exception annotations

#define may_err  noexcept(false)
#define will_err noexcept(false)

//------------------------------------------------------------------------------
// debug support

#include <assert.h>
#define EXPECT_(cond) assert(cond);
#define ENSURE_(cond) assert(cond);

//------------------------------------------------------------------------------
#endif
// eof
