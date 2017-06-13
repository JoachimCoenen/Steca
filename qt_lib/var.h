// (qt_lib)

#ifndef QT_LIB_VAR_H
#define QT_LIB_VAR_H

#include <dev_lib/inc/defs_h.h>
#include "tim.h"
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
#endif
// eof
