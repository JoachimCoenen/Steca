// (lib/qt)

#pragma once
#include "../defs.hpp"
#include <QTabWidget>

namespace l_qt {
//------------------------------------------------------------------------------

struct panel;

dcl_sub_(tabs, QTabWidget)
  tabs();
  set_(addTab, (panel*, strc title, QWidget* tabBtn = nullptr));
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
