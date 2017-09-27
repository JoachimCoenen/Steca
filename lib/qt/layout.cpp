// (qt_lib)

#include "layout.hpp"
#include "inc/defs.inc"
#include "split.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

box& grid::hb(uint row, uint col) {
  auto hb = new hbox;
  addLayout(hb, int(row), int(col));
  return *hb;
}

box& grid::vb(uint row, uint col) {
  auto vb = new vbox;
  addLayout(vb, int(row), int(col));
  return *vb;
}

grid& grid::gr(uint row, uint col) {
  auto gr = new grid;
  addLayout(gr, int(row), int(col));
  return *gr;
}

grid::ref grid::add(QWidget* wgt, uint row, uint col)
  SET_(addWidget(NEEDED_(wgt), int(row), int(col)))

grid::ref grid::addRowStretch()
  SET_(setRowStretch(rowCount(), 0))

grid::ref grid::addColStretch()
  SET_(setColumnStretch(columnCount(), 0))

grid::grid() {
  base::setSpacing(2);
  base::setMargin(2);
}

//------------------------------------------------------------------------------

box::box() : base(QBoxLayout::LeftToRight) {
  base::setSpacing(2);
  base::setMargin(2);
}

box& box::hb() {
  auto hb = new hbox;
  addLayout(hb);
  return *hb;
}

box& box::vb() {
  auto vb = new vbox;
  addLayout(vb);
  return *vb;
}

grid& box::gr() {
  auto gr = new grid;
  addLayout(gr);
  return *gr;
}

split& box::hs() {
  auto hs = new hSplit;
  add(hs);
  return *hs;
}

split& box::vs() {
  auto s = new vSplit;
  add(s);
  return *s;
}

box::ref box::margin(uint n) {
  setMargin(int(n)); RTHIS
}

box::ref box::spacing(uint n) {
  setMargin(int(n)); RTHIS
}

box::ref box::add(QWidget* wgt) {
  EXPECT_(wgt)
  addWidget(wgt); RTHIS
}

box::ref box::addStretch()
  SET_(base::addStretch(0))

box::ref box::align(QLayout& l, Qt::Alignment a)
  SET_(base::setAlignment(&l, a))

box::ref box::align(QWidget* w, Qt::Alignment a)
SET_(base::setAlignment(w, a))

//------------------------------------------------------------------------------

hbox::hbox() {
  base::setDirection(QBoxLayout::LeftToRight);
}

vbox::vbox() {
  base::setDirection(QBoxLayout::TopToBottom);
}

//------------------------------------------------------------------------------

panel::panel() {}

static void setPanelLayout(panel* p, QLayout *l) {
  EXPECT_(p) EXPECT_(l)
  auto *ol = p->layout();
  check_or_err_(!ol, "panel already has a box");
  p->setLayout(l);
}

box& panel::hb() {
  auto hb = new hbox;
  setPanelLayout(this, hb);
  return *hb;
}

box& panel::vb() {
  auto vb = new vbox;
  setPanelLayout(this, vb);
  return *vb;
}

grid& panel::gr() {
  auto gr = new grid;
  setPanelLayout(this, gr);
  return *gr;
}

//------------------------------------------------------------------------------
}
// eof

