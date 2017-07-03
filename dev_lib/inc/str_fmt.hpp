// (dev_lib)

#pragma once

#include "../defs.hpp"
#include <sstream>

namespace l {
//------------------------------------------------------------------------------

dcl_reimpl_(str_fmt, std::stringstream)
  using base::str;

  template <typename T>
  Self& cat(T const& t) SET_(*this << t)

  template <typename T, typename... R>
  Self& cat(T const& t, R const&... r) SET_(cat(t); cat(r...))
dcl_end

#define CAT(...) (l::str_fmt().cat(__VA_ARGS__).str())

str trim(strc);

//------------------------------------------------------------------------------
}
// eof
