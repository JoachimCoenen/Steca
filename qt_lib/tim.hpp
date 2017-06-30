// (qt_lib)

#ifndef QT_LIB_TIM_HPP
#define QT_LIB_TIM_HPP

#include <dev_lib/inc/defs.hpp>
#include <dev_lib/inc/num.hpp>

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
