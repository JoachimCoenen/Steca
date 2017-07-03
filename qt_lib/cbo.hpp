// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
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
// eof
