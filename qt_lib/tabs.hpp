// (qt_lib)

#ifndef QT_LIB_TABS_HPP
#define QT_LIB_TABS_HPP

#include <dev_lib/inc/defs.hpp>
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
