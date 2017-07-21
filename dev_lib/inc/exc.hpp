// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "str_fmt.hpp"  // typically, who uses err(...), needs CAT
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

#define guard_err_(msg_, ...)       \
  try { __VA_ARGS__ }               \
  catch (l::exc::rc e) {            \
    l::err(CAT(msg_, ": ", e.msg)); \
  } catch (std::exception& e) {     \
    l::err(CAT(msg_, ": ", e.what())); \
  }

//------------------------------------------------------------------------------
}
// eof