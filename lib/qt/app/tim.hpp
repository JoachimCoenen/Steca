// (lib/qt)

/** @file
 * time : milliseconds and now
 */

#pragma once

#include "../defs.hpp"
#include <lib/dev/inc/num.hpp>

namespace l_qt {
//------------------------------------------------------------------------------

use_int_(msec_t, int64, {})

dcl_(tim) COMPARABLE EQNE
  /// milliseconds since epoch
  atr_(msec_t, msec);

  /// initialized to current time
  tim();
  tim(msec_t);
dcl_end

//------------------------------------------------------------------------------
}
// eof
