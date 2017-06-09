// (app_lib)

#ifndef APP_LIB_TYP_QT_CHK_H
#define APP_LIB_TYP_QT_CHK_H

#include "../inc/defs_h.h"
#include <QCheckBox>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

dcl_sub_(chk, QCheckBox)
  chk();
  chk(strc);
  chk(act&);

  set_(text, (strc))
  set_(action, (act*))
  set_(check, (bool = true))

protected:
  QMetaObject::Connection con1, con2;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
