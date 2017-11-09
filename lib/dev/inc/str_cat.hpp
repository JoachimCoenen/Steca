// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "str.hpp" // who needs str_cat, needs str also
#include <sstream>

namespace l {
//------------------------------------------------------------------------------

dcl_reimpl_(str_cat, std::stringstream)
  template <typename T>
  Self& cat(T const& t) SET_(*this << t)

  template <typename T, typename... R>
  Self& cat(T const& t, R const&... r) SET_(cat(t); cat(r...))

  ::str val();
dcl_end

#define CAT(...) (l::str_cat().cat(__VA_ARGS__).val())

//------------------------------------------------------------------------------
}
// eof
