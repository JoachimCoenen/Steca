// (qt_lib)

#include "split.h"
#include <dev_lib/inc/defs_cpp.h>

#include "layout.h"

namespace l_qt {
//------------------------------------------------------------------------------

static split& _hvs(split* s, split* t) {
  EXPECT_(s) EXPECT_(t)
  t->addWidget(s);
  return *s;
}

split& split::hs() {
  return _hvs(new hSplit, this);
}

split& split::vs() {
  return _hvs(new vSplit, this);
}

split& split::add(QWidget* wgt) {
  EXPECT_(wgt)
  addWidget(wgt);
  RT
}

panel& split::addPanel() {
  auto p = new panel;
  add(p);
  return *p;
}

split::split() {}

//------------------------------------------------------------------------------

hSplit::hSplit() {
  setOrientation(Qt::Horizontal);
}

vSplit::vSplit() {
  setOrientation(Qt::Vertical);
}

//------------------------------------------------------------------------------
}
// eof
