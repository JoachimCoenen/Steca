// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QRadioButton>

namespace l_qt {
//------------------------------------------------------------------------------

/// radio button
dcl_sub_(rio, QRadioButton)
  rio(strc);

  set_(text, (strc));
dcl_end

//------------------------------------------------------------------------------
}
// eof
