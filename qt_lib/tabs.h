// (app_lib)

#ifndef APP_LIB_TYP_QT_TABS_H
#define APP_LIB_TYP_QT_TABS_H

#include "../inc/defs_h.h"
#include <QTabWidget>

namespace l_qt {
//------------------------------------------------------------------------------

struct panel;

dcl_sub_(tabs, QTabWidget)
  tabs();
  set_(addTab, (panel*, strc title, QWidget* tabBtn = nullptr))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
