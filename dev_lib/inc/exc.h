// (dev_lib)

#ifndef DEV_LIB_EXC_H
#define DEV_LIB_EXC_H

#include "defs_h.h"
#include <exception>

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(exc, std::exception)
  atr_(str,  msg);
  atr_(bool, silent);

  exc(strc) noexcept;
dcl_end

// throw an exception
[[noreturn]] void err()     will_err;
[[noreturn]] void err(strc) will_err;

template <typename T> str cat(strc s, T const& t) {
  return s + ' ' + std::to_string(t);
}

template <typename T>
[[noreturn]] void err(strc msg, T const& t) will_err {
  throw exc(cat(msg, t));
}

// runtime check
#define check_or_err_(cond, ...) \
  if (!(cond)) l::err(__VA_ARGS__)

//------------------------------------------------------------------------------
}
#endif
// eof
