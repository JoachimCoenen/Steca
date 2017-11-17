// (lib/qt)

/**@file
 * A tab widget.
 */

#pragma once

#include "../defs.hpp"
#include <QTabWidget>

namespace l_qt {
//------------------------------------------------------------------------------

struct panel;

/// a tab widget
dcl_sub_(tabs, QTabWidget)
  tabs();
  /// add a panel as a tab
  set_(addTab, (panel*, strc title, QWidget* tabBtn = nullptr));
dcl_end

//------------------------------------------------------------------------------
}
// eof
