// (app_lib)

#ifndef APP_LIB_TYP_QT_CBO_H
#define APP_LIB_TYP_QT_CBO_H

#include "../inc/defs_h.h"
#include <QComboBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(cbo, QComboBox)
  cbo();
  cbo(str_lst_rc);

  set_(add, (strc))
  set_(add, (str_lst_rc))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
