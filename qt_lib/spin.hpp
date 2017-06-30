// (qt_lib)

#ifndef QT_LIB_SPIN_HPP
#define QT_LIB_SPIN_HPP

#include <dev_lib/inc/defs.hpp>
#include <QDoubleSpinBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(spin, QDoubleSpinBox)
  spin();

  set_(decimals,   (uint));
  set_(digitWidth, (uint));
  set_(min, (real));
  set_(max, (real));
  set_(val, (real));

  mth_(real, val, ());
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
