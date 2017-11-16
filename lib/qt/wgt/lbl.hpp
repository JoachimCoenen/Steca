// (lib/qt)

#pragma once
#include "../defs.hpp"
#include <QLabel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lbl, QLabel)
  lbl();
  lbl(strc);

  set_(text, (strc));
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
