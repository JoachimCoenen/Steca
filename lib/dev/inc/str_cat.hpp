// (lib/dev)

/** @file
 * a string catenator
 */

#pragma once

#include "../defs.hpp"
#include "str.hpp" // complimentary include: who needs str_cat, needs str also
#include <sstream>

namespace l {
//------------------------------------------------------------------------------

dcl_reimpl_(str_cat, std::stringstream)
  /// appends a string form of @c t
  template <typename T>
  Self& cat(T const& t) SET_(*this << t)

  /// appends string forms of @c ...
  template <typename T, typename... R>
  Self& cat(T const& t, R const&... r) SET_(cat(t); cat(r...))

  /// returns the catenated string
  ::str val();
dcl_end

/** \def CAT(...)
 * a utility macro that concatenates string representations of its arguments
 */
#define CAT(...) (l::str_cat().cat(__VA_ARGS__).val())

//------------------------------------------------------------------------------
}
// eof
