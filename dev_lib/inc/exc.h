// (dev_lib)

#ifndef DEV_LIB_EXC_H
#define DEV_LIB_EXC_H

#include "defs_h.h"
#include "str_fmt.h"
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

// runtime check
#define check_or_err_(cond, msg) \
  if (!(cond)) l::err(msg)

//------------------------------------------------------------------------------
}
#endif
// eof
