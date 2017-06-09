// (app_lib)

#ifndef APP_LIB_TYP_QT_RIO_H
#define APP_LIB_TYP_QT_RIO_H

#include "../inc/defs_h.h"
#include <QRadioButton>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(rio, QRadioButton)
  rio();
  rio(strc);

  set_(text, (strc))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
