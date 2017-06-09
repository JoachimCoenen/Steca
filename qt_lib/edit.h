// (app_lib)

#ifndef APP_LIB_TYP_QT_EDIT_H
#define APP_LIB_TYP_QT_EDIT_H

#include "../inc/defs_h.h"
#include <QLineEdit>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(edit, QLineEdit)
  edit();
  set_(ro, (bool = true))
  set_(text, (strc))
  mth_(str, text, ())
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
