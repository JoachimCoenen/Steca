// (qt_lib)

#pragma once

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
// eof
