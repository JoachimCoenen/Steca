// (lib/qt)

#include "split.hpp"
#include "../defs.inc"
#include "../win/layout.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

static split& _hvs(split* s, split* t) {
  NEED_(t)->add(NEED_(s));
  return *s;
}

split& split::hs() {
  return _hvs(new hSplit, this);
}

split& split::vs() {
  return _hvs(new vSplit, this);
}

split& split::add(QWidget* wgt)
  SET_(addWidget(NEED_(wgt)))

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
// eof DOCS
