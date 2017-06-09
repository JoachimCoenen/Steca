// (app_lib)

#ifndef APP_LIB_TYP_QT_SPIN_H
#define APP_LIB_TYP_QT_SPIN_H

#include "../inc/defs_h.h"
#include <QDoubleSpinBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(spin, QDoubleSpinBox)
  spin();

  set_(decimals, (uint))
  set_(digitWidth, (uint))
  set_(min, (real))
  set_(max, (real))
  set_(val, (real))

  mth_(real, val, ())
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
