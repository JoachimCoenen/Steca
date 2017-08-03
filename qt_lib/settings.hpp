// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
#include <QSettings>
#include "var.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(Settings, QSettings)
  cst_(pcstr, GROUP_MAINWIN);
  cst_(pcstr, GEOMETRY);
  cst_(pcstr, STATE);

  Settings(strc group = str::null);
 ~Settings();

  mth_(var, readVar, (strc key, var::rc def));
  set_(saveVar, (strc key, var::rc));

  mth_(bool, readBool, (strc key, bool def = false));
  set_(saveBool, (strc key, bool));

  mth_(int, readInt, (strc key, int def = 0));
  set_(saveInt, (strc key, int));

  mth_(real, readReal, (strc key, qreal def = 0));
  set_(saveReal, (strc key, qreal));

  mth_(str, readStr, (strc key, strc def = str::null));
  set_(saveStr, (strc key, strc));
dcl_end

//------------------------------------------------------------------------------
}
// eof
