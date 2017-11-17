// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QLineEdit>

namespace l_qt {
//------------------------------------------------------------------------------

/// an edit control
dcl_sub_(edit, QLineEdit)
  edit();

  /// retrieve text
  mth_(str, text, ());
  /// set text
  set_(text, (strc));

  /// change read/only status
  set_(ro, (bool = true));
dcl_end

//------------------------------------------------------------------------------
}
// eof
