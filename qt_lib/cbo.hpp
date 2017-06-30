// (qt_lib)

#ifndef QT_LIB_CBO_HPP
#define QT_LIB_CBO_HPP

#include <dev_lib/inc/defs.hpp>
#include "str_inc.hpp"

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
