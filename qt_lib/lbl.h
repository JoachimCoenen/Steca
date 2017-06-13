// (qt_lib)

#ifndef QT_LIB_LBL_H
#define QT_LIB_LBL_H

#include <dev_lib/inc/defs_h.h>
#include <QLabel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lbl, QLabel)
  lbl(strc);

  set_(text, (strc));
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
