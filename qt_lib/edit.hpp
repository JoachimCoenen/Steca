// (qt_lib)

#ifndef QT_LIB_EDIT_HPP
#define QT_LIB_EDIT_HPP

#include <dev_lib/inc/defs.hpp>
#include <QLineEdit>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(edit, QLineEdit)
  edit();

  mth_(str, text, ());
  set_(text, (strc));
  set_(ro, (bool = true));
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
