// (qt_lib)

#pragma once
#include <dev_lib/defs.hpp>
#include "tim.hpp"
#include <QVariant>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(var, QVariant)
  var();
  var(pcstr);
  var(strc);
  var(QVariant const&);

  bol_(isNum,  ());
  mth_(int,  toInt,  ()) may_err;
  mth_(str,  toStr,  ());
  mth_(real, toReal, ()) may_err;
  mth_(tim,  toTim,  ());
dcl_end

int cmp_int(var::rc,  var::rc);
int cmp_str(var::rc,  var::rc);
int cmp_real(var::rc, var::rc);
int cmp_tim(var::rc,  var::rc);

//------------------------------------------------------------------------------
}
// eof
