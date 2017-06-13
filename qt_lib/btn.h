// (qt_lib)

#ifndef QT_LIB_BTN_H
#define QT_LIB_BTN_H

#include <dev_lib/inc/defs_h.h>
#include <QPushButton>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(btn, QPushButton)
  btn(strc);
  set_(text, (strc));
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
