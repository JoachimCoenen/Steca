// (qt_lib)

#ifndef QT_LIB_TIM_H
#define QT_LIB_TIM_H

#include <dev_lib/inc/defs_h.h>
#include <dev_lib/inc/num.h>

namespace l_qt {
//------------------------------------------------------------------------------

use_num_(msec_t, int64, {})

dcl_(tim) COMPARABLE EQ_NE
  atr_(msec_t, msec); // msec since epoch

  tim(); // now
  tim(msec_t);
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
