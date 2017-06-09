// (app_lib)

#ifndef APP_LIB_TYP_QT_TIM_H
#define APP_LIB_TYP_QT_TIM_H

#include "../inc/defs_h.h"
#include "../inc/num.h"

namespace l_qt {
//------------------------------------------------------------------------------

use_num_ (msec_t, int64, {})

dcl_(tim) COMPARABLE EQ_NE
  atr_(msec_t, msec)  // since epoch

  tim();              // now
  tim(msec_t);
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
