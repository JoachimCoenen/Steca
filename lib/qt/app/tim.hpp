// (qt_lib)

#pragma once
#include "../defs.hpp"
#include <lib/dev/inc/num.hpp>

namespace l_qt {
//------------------------------------------------------------------------------

use_int_(msec_t, int64, {})

dcl_(tim) COMPARABLE EQ_NE
  atr_(msec_t, msec); // msec since epoch

  tim(); // now
  tim(msec_t);
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
