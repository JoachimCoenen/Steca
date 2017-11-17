// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QLabel>

namespace l_qt {
//------------------------------------------------------------------------------

/// icon
dcl_sub_(ico, QLabel)
  ico();
  ico(strc);

  /// set a pixmap from resource
  set_(pix, (strc));
dcl_end

//------------------------------------------------------------------------------
}
// eof
