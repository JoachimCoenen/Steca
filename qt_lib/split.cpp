// (qt_lib)

#include "split.hpp"
#include "inc/defs.inc"
#include "layout.hpp"

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

split& split::add(QWidget* wgt)
  SET_(addWidget(NEEDED_(wgt)))

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
