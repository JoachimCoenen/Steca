// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
#include "tim.hpp"

#include <QVariant>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(var, QVariant)
  var();
  var(QVariant const&);

  mth_(bool, isNum,  ());
  mth_(int,  toInt,  ());
  mth_(str,  toStr,  ());
  mth_(real, toReal, ());
  mth_(tim,  toTim,  ());
dcl_end

int cmp_int(var::rc,  var::rc);
int cmp_str(var::rc,  var::rc);
int cmp_real(var::rc, var::rc);
int cmp_tim(var::rc,  var::rc);

//------------------------------------------------------------------------------
}
// eof
