// (app_lib)

#ifndef APP_LIB_TYP_QT_LBL_H
#define APP_LIB_TYP_QT_LBL_H

#include "../inc/defs_h.h"
#include <QLabel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lbl, QLabel)
  lbl(strc);
  set_(text, (strc))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
