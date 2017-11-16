// (lib/qt)

#pragma once
#include "../defs.hpp"
#include <QComboBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(cbo, QComboBox)
  cbo();
  cbo(str_vec const&);

  set_(add, (strc));
  set_(add, (str_vec const&));
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
