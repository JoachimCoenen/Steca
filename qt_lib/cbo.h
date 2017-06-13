// (qt_lib)

#ifndef QT_LIB_CBO_H
#define QT_LIB_CBO_H

#include <dev_lib/inc/defs_h.h>
#include "str_inc.h"
#include <QComboBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(cbo, QComboBox)
  cbo();
  cbo(str_vec::rc);

  set_(add, (strc));
  set_(add, (str_vec::rc));
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
