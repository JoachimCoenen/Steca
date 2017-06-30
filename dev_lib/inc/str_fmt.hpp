// (dev_lib)

#ifndef DEV_LIB_STR_FMT_HPP
#define DEV_LIB_STR_FMT_HPP

#include "defs.hpp"
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
#endif
// eof
