// (app_lib)

#include "tabs.h"
#include "../inc/defs_cpp.h"
#include "layout.h"
#include <QTabBar>

namespace l_qt {
//------------------------------------------------------------------------------

tabs::tabs() {}

tabs::ref tabs::addTab(panel* p, strc title, QWidget* tabBtn) {
  EXPECT_(p)
  auto n = base::addTab(p, title);
  if (tabBtn)
    base::tabBar()->setTabButton(n, QTabBar::RightSide, tabBtn);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
