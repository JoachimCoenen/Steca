// (lib/dev)

/** @file
 * Exception type (the only kind that is used here) to signal errors.
 * Macros for runtime checks.
 */

#pragma once

#include "../defs.hpp"
#include "str_cat.hpp"  // complimentary include: who needs err(), needs CAT also
#include <exception>

namespace l {
//------------------------------------------------------------------------------

dcl_sub_(exc, std::exception)
  atr_(str,  msg);    ///< message
  atr_(bool, silent); ///< a "silent" flag - the message will not be presented to the user

  exc(strc, bool silent) noexcept;
dcl_end

/// construct and throw an exception with an empty message
[[noreturn]] void err() will_err;

/// construct and throw an exception with a message
[[noreturn]] void err(strc)  will_err;

/// construct and throw an exception with a message
[[noreturn]] void err(pcstr) will_err;

/// construct and throw a "silent" exception with an empty message
[[noreturn]] void errSilent()      will_err;

/// construct and throw a "silent" exception with a message
[[noreturn]] void errSilent(strc)  will_err;

/// construct and throw a "silent" exception with a message
[[noreturn]] void errSilent(pcstr) will_err;

/** \def check_or_err_(cond, msg)
 * runtime check: @c cond must be true, or an exception with @c msg will be thrown
 */
#define check_or_err_(cond, msg) \
  ((cond) || (l::err(msg), true))

/** \def wrap_err_msg_(msg, ...)
 * catches exceptions thrown from @c ... and rethrows them with @c msg prepended
 */
#define wrap_err_msg_(msg_, ...)       \
  try { __VA_ARGS__ }               \
  catch (l::exc::rc e) {            \
    l::err(CAT(msg_, ": ", e.msg)); \
  } catch (std::exception& e) {     \
    l::err(CAT(msg_, ": ", e.what())); \
  }

//------------------------------------------------------------------------------
}
// eof
