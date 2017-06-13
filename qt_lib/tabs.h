// (qt_lib)

#ifndef QT_LIB_TABS_H
#define QT_LIB_TABS_H

#include <dev_lib/inc/defs_h.h>
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
#endif
// eof
