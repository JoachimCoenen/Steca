// (qt_lib)

#include "tabs.h"
#include <dev_lib/inc/defs_cpp.h>
#include "layout.h"
#include "str_inc.h"

#include <QTabBar>

namespace l_qt {
//------------------------------------------------------------------------------

tabs::tabs() {}

tabs::ref tabs::addTab(panel* p, strc title, QWidget* tabBtn) {
  EXPECT_(p)
  auto n = base::addTab(p, toQt(title));
  if (tabBtn)
    base::tabBar()->setTabButton(n, QTabBar::RightSide, tabBtn);
  RTHIS
}

//------------------------------------------------------------------------------
}
// eof
