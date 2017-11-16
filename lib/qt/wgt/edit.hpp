// (lib/qt)

#pragma once
#include "../defs.hpp"
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
// eof DOCS
