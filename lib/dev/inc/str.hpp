// (lib/dev)

/** @file
 * the @c str (string) class - an extension of <code>std::string</code>
 */

#pragma once

#include "../defs.hpp"
#include <string>

//------------------------------------------------------------------------------

dcl_sub_(str, std::string)
  UB3_(base, c_str, length)

  str() {}
  str(base const& that) : base(that) {}

  bol_(isEmpty, ()) RET_(base::empty())

  mth_(flt32, asFlt, ()) may_err;         ///< parse as flt32

  static str const null;                  ///< an empty string

  mth_(str, trimmed, ());                 ///< trimmed whitespace, left and right

  fry_(str, num, (int));                  ///< format @c int as a string
  fry_(str, num, (uint));                 ///< format @c uint as a string

private:
  bool empty() const; // disallow @c empty() - @c isEmpty() is a better name
dcl_end

/// @cond
namespace std {

template <> struct hash<str> {
  size_t operator()(strc s) const;
};

}
/// @endcond

//------------------------------------------------------------------------------
// eof
