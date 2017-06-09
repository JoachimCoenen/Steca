// (app_lib)

#ifndef APP_LIB_TYP_QT_BTN_H
#define APP_LIB_TYP_QT_BTN_H

#include "../inc/defs_h.h"
#include <QPushButton>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(btn, QPushButton)
  btn(strc);
  set_(text, (strc))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
