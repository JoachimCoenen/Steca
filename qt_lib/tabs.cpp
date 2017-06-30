// (qt_lib)

#include "tabs.hpp"
#include <dev_lib/inc/defs.inc>

#include "layout.hpp"
#include "str_inc.hpp"

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
