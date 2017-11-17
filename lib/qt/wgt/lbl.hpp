// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QLabel>

namespace l_qt {
//------------------------------------------------------------------------------

/// text label
dcl_sub_(lbl, QLabel)
  lbl();
  lbl(strc);

  set_(text, (strc));
dcl_end

//------------------------------------------------------------------------------
}
// eof
