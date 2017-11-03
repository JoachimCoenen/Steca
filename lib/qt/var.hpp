// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include "tim.hpp"
#include <QVariant>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(var, QVariant)
  var();
  var(int);
  var(uint);
  var(pcstr);
  var(strc);
  var(flt32);
  var(flt64);
  var(QVariant const&);

  bol_(isNum,  ());
  bol_(isReal, ());

  mth_(int,  toInt,  ()) may_err;
  mth_(uint, toUInt, ()) may_err;
  mth_(str,  toStr,  ());
  mth_(real, toReal, ()) may_err;
  mth_(tim,  toTim,  ());

  using cmpFun = int(rc, rc);

  static int cmp_int(var::rc,  var::rc);
  static int cmp_str(var::rc,  var::rc);
  static int cmp_real(var::rc, var::rc);
  static int cmp_tim(var::rc,  var::rc);
dcl_end

//------------------------------------------------------------------------------
}
// eof
