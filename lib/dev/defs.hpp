// (lib/dev)

/** @file

This file contains common definitions and should be included, directly or
indirectly probably into every /header/ file in the project.

Specifically, here find you a set of macros for declarations of structures.
They are based on a number of fundamental assumptions and choices:

- @b Public visibility, therefore the preference to use @c struct over @c
class. Both are equivalent in C++, except for the default visibility of members
(@c public resp. @c private). Public visibility should be the default.
- @b Immutability. If data members are publicly visible, they should be
read-only (marked as @c const) and mutated after creation only in exceptional,
explicitely allowed circumstances.
- @b Access methods. Since data members are publicly visible and immutable,
there is no need for 'getters'.
- @b Sharing. Immutable data are shared (passed around) by const references and
shared (reference counted) pointers. Use of move semantics is therefore discouraged.

Since most types are immutable, it is necessary to make them mutable in cases
when a value needs to be changed. That, of course, must be done judiciously,
above all it is necessary to ensure that the modifying agent is the sole one
that has access to the data, or that changing the data has no unexpected impact
to others that may have access to the data. Typically, one would modify:
- data members in a constructor
- local data that are not shared

There are a set of macros that help with and also enforce the above rules in
declarations of data types. The macros inject @c const modifiers. They are:
- @c dcl_, @c dcl_base_, @c dcl_sub_, @c dcl_sub2_, @c  dcl_reimpl_,
  @c dcl_reimpl2_, @c dcl_end: delimit each declaration
- @c atr_, @c ptr_, @c ref_, @c cst_: these declare data members
- @c fry_, @c mth_, @c mth_mut_, @c bol_, @c bol_mut_, @c cop_, @c voi_,
  @c mut_, @c set_ - declare methods

The following templates allow lifting (casting away) the const-ness of a type
or a data element: @c mut_typ, @c mut, @c mutp.

A few very useful types are defined or forward-declared here:
- @c uint, @c uint8, @c int8, @c uint16, @c int16, @c uint32, @c int32, @c
uint64, @c int64 - signed and unsigned integers of various sizes
- @c sz_t - a size type
- @c flt32, @c flt64, @c real - floating point numbers (real ~ flt64)
- @c bol_vec, @c int_vec, @c uint_vec, @c real_vec - vectors
- @c str, @c str_vec - a string type and a string vector
- @c strc, @c pcstr - a const reference to str (read: (str)c / st(rc)) and a
  C-style string pointer

Finally, a few macros:
- @c may_err, @c will_err - exception annotation
- @c EXPECT_, @c ENSURE_, @c NEED_(cond) - assertions

The library is contained in a namespace @c l ((l)ibrary).

Note: Yoda want we to be, prefer we thus <code>type const</code> over
<code>const type</code>.

Note: the use of the underscore '_'. A leading underscore is sometimes used to
indicate that a member (method, attribute) is not for outside use. The trailing
underscore is used in macro names to:
- avoid name clashes with normal identifiers
- to force an "optical space" in the code (z.B. <code>dcl_(name)</code> reads
better than <code>dcl (name)</code> or <code>dcl(name)</code>).

Note: Yoda wants you to be brief. Therefore well-known and oft-used names are
used in their three-letter form. Some even have a one-letter form. And a few
are two-letter. Zusammengestzte Wörter können aus diesen Partikel gebildet
werden.

Here is a vocabulary:

- @c arg argument
- @c atr attribute
- @c avg average (noun or verb)
- @c beg begin
- @c bol boolean
- @c cbo combo
- @c col, @c cl column
- @c cst, @c c const
- @c dbl double
- @c dcl declare / declaration
- @c def define / definition
- @c end end
- @c err error / to err
- @c exc exception
- @c flt float
- @c fry factory
- @c int integer
- @c lst list
- @c mth method
- @c mut mutable / mutate
- @c no, number (ordinal)
- @c num, @c n number (how many)
- @c par parameter
- @c pos position
- @c ptr, @c p pointer, @c pc pointer to a constant
- @c r reference, @c rc reference to a constant
- @c row, @c rw row
- @c rge range
- @c set set / setter
- @c str string
- @c res result
- @c ret return
- @c sh  shared
- @c shp shared pointer
- @c shr shared reference
- @c str, @c s string
- @c sub sub(structure, etc.)
- @c tgl toggle (as in a button or action)
- @c trg trigger (as in a button or action)
- @c sz  size
- @c tx  text
- @c typ type
- @c val, @c v value
- @c var variant / variable
- @c vec vector
- @c voi void

Identifiers ending in <code>-s</code> (the plural form) denote collections.
*/

#pragma once

#ifdef __clang__

#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wreserved-id-macro"

// allow Doxygen @file annotations
#pragma GCC diagnostic ignored "-Wdocumentation-unknown-command"

#endif

/** Delimit a clang-specific source text, for example:
 * @code
 * switch (...) {
 * case ...:
 *   dlgInfo(mainWin, msg);
 *   _if_clang_([[clang::fallthrough]];)
 * default:
 *   ...
 * }
 * @endcode
 */
#ifdef __clang__
  #define _if_clang_(...) __VA_ARGS__
#else
  #define _if_clang_(...)
#endif

/// Macros with templates may need a "non-comma" comma.
#define COMMA ,

//------------------------------------------------------------------------------

/**
 * Declared structures have these, very useful, self-referencing types:
 * - @c Self: an alias for its own type.
 * - @c rc: a const reference to @c Self (@c rc = (r)eference (c)onst)
 * - @c ref: a mutable reference to @c Self
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
#define _self_types_(s)
#else
#define _self_types_(s)     \
  using Self = s;           \
  using rc   = Self const&; \
  using ref  = Self&;
#endif

/// A @c struct declaration begins with <code>dcl_(name)</code>.
#define dcl_(s) \
  struct s { _self_types_(s)

/// A polymorphic base @c struct declaration begins with <code>dcl_base_(name)</code>.
// begin declaration of polymorphic struct (the base type)
#define dcl_base_(s) \
  dcl_(s) virtual ~s();

/** A derived ("sub") structure declaration begins with <code>dcl_sub_(name, base)</code>.
- The inheritance has public visibility.
- @c base is an alias to the super-structure type.
*/
#define dcl_sub_(s, b) \
  struct s : b { _self_types_(s) using base = b;

/**
 * Multiple inheritance is discouraged, for known reasons. But inheriting from
 * two structures is, in specific circumstances, useful and necessary. Such
 * declaration begins with <code>dcl_sub2_(name, other_base, base)</code>.
 * - The inheritance has public visibility.
 * - @c base is an alias to the (second) super-structure type.
 */
// begin a declaration of sub-struct from two bases
#define dcl_sub2_(s, b1, b2) \
  struct s : b1, b2 { _self_types_(s) using base = b2;

/** "Reimplementation" is inheritance with protected visibility. The reimplementor
 * decides which methods from the base struct to simply re-export (@c using) and
 * which to re-implement (that allows, for example, type conversion).
 * A reimplmented structure declaration begins with <code>dcl_reimpl_(name, base)</code>.
 * - @c base_rc() provides (immutable) access to the base object.
 */
#define dcl_reimpl_(s, b) \
  struct s : protected b { _self_types_(s) using base = b; \
    base const& base_rc() const { return *static_cast<base const*>(this); }

/** Reimplementing from two bases. */
#define dcl_reimpl2_(s, b1, b2) \
  struct s : b1, protected b2 { _self_types_(s) using base = b2; \
    base const& base_rc() const { return *static_cast<base const*>(this); }

/** Each declaration ends with <code>dcl_end_</code>. */
#define dcl_end \
  };

/// immutable attribute
#define atr_(typ, name) typ const name
/// immutable pointer to immutable data
#define ptr_(typ, name) typ const * const name
/// reference to immutable data
#define ref_(typ, name) typ const &       name

/// a static constant
#define cst_(typ, name) static atr_(typ, name)

/// a factory
#define fry_(typ, mth, args) static typ mth args

/// a method that does not mutate the object's state
#define mth_(typ, mth, args)              typ mth args const

/// a method that may mutate the object's state
#define mth_mut_(typ, mth, args)          typ mth args

/// a predicate (@c bool non-mutating method)
#define bol_(mth, args)                   mth_(bool, mth, args)
/// a mutating method that returns a truth value (i.e. a success indicator)
#define bol_mut_(mth, args)               mth_mut_(bool, mth, args)

/// a conversion operator
#define cop_(op)                          operator op() const

/// a void, non-mutating method
#define voi_(mth, args)                   mth_(void, mth, args)
/// a mutating void method
#define mut_(mth, args)                   mth_mut_(void, mth, args)

/// a "setter" method: allows chaining of calls (returns <code>*this</code>)
#define set_(mth, args)                   mth_mut_(ref, mth, args)

/// a <code>return *this</code> helper; used in @c set_ methods
#define RTHIS return *this;

/// make a structure three-way comparable ...
#define COMPARABLE  int compare(rc) const;

/// ... with equality ops ...
#define EQNE bool operator==(rc) const; \
             bool operator!=(rc) const;

/// ... with inequality ops
#define LGTE bool operator< (rc) const; \
             bool operator<=(rc) const; \
             bool operator> (rc) const; \
             bool operator>=(rc) const;

/// a helper to implement a body of non-mutating method
#define RET_(expr) { return expr; }

/// a helper to implement a body of @c set_
#define SET_(...) { __VA_ARGS__; RTHIS }

/** These are helpers to generate <code>using base::...</code> directives.
 * A better solution would use variadic macros, but those fail on Windows 7,
 * which we still must support
 */
#define UB_(mth) using base::mth;
#define UB9_(a, b, c, d, e, f, g, h, i) UB_(a) UB_(b) UB_(c) UB_(d) UB_(e) UB_(f) UB_(g) UB_(h) UB_(i)
#define UB8_(a, b, c, d, e, f, g, h)    UB_(a) UB_(b) UB_(c) UB_(d) UB_(e) UB_(f) UB_(g) UB_(h)
#define UB7_(a, b, c, d, e, f, g)       UB_(a) UB_(b) UB_(c) UB_(d) UB_(e) UB_(f) UB_(g)
#define UB6_(a, b, c, d, e, f)          UB_(a) UB_(b) UB_(c) UB_(d) UB_(e) UB_(f)
#define UB5_(a, b, c, d, e)             UB_(a) UB_(b) UB_(c) UB_(d) UB_(e)
#define UB4_(a, b, c, d)                UB_(a) UB_(b) UB_(c) UB_(d)
#define UB3_(a, b, c)                   UB_(a) UB_(b) UB_(c)
#define UB2_(a, b)                      UB_(a) UB_(b)
#define UB1_(a)                         UB_(a)

//------------------------------------------------------------------------------

/// makes a type mutable
template <typename T> struct mut_typ            { using typ = T; };
template <typename T> struct mut_typ<T const>   { using typ = T; };

/// makes a pointer mutable
template <typename T> T const*& mut(T const*const& t) \
  RET_(const_cast<T const*&>(t))

/// makes a value mutable
template <typename T> T& mut(T const& t) \
  RET_(const_cast<T&>(t))

/// makes a pointed-to value mutable
template <typename T> T* mutp(T const* t) \
  RET_(const_cast<T*>(t))

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

/// size type
#ifndef NDEBUG
  struct sz_t {
    explicit sz_t(uint val_) : val(val_) {}
    operator uint() const RET_(val)
    sz_t& operator++()    SET_(++val)
    sz_t& operator--()    SET_(--val)
    sz_t  operator++(int) RET_(sz_t(val++))
    sz_t  operator--(int) RET_(sz_t(val--))
  protected:
    uint val;
  };
#else
  using sz_t = uint;
#endif

/// 32 and 64 bit floating-point numbers
typedef float  flt32;
typedef double flt64;

/// a default floating-point type
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
// strings

struct str;
struct str_vec;

using strc  = str const&;
using pcstr = char const*;

//------------------------------------------------------------------------------
// exception annotations

/// annotation for a method that may throw an exception
#define may_err  noexcept(false)

/// annotation for a method that always throws an exception
#define will_err noexcept(false)

//------------------------------------------------------------------------------
// debug support

#include <assert.h>

/// a (debug time) pre-condition assertion
#define EXPECT_(cond) assert(cond);

/// a (debug time) invariant or a post-condition assertion
#define ENSURE_(cond) assert(cond);

/// an assertion that passes on its value (if it does not fail)
#define NEED_(cond)  (assert(cond), cond)

//------------------------------------------------------------------------------
// eof
