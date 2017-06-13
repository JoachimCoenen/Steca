// (qt_lib)

#include "layout.h"
#include <dev_lib/inc/defs_cpp.h>

#include "split.h"

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

grid::ref grid::add(QWidget* wgt, uint row, uint col) {
  EXPECT_(wgt)
  addWidget(wgt, int(row), int(col)); RT
}

grid::ref grid::addRowStretch() {
  setRowStretch(rowCount(), 0); RT
}

grid::ref grid::addColStretch() {
  setColumnStretch(columnCount(), 0); RT
}

grid::grid() {
  base::setSpacing(2);
  base::setMargin(2);
}

//------------------------------------------------------------------------------

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
  setMargin(int(n)); RT
}

box::ref box::spacing(uint n) {
  setMargin(int(n)); RT
}

box::ref box::add(QWidget* wgt) {
  EXPECT_(wgt)
  addWidget(wgt); RT
}

box::ref box::addStretch() {
  addStretch(); RT
}

box::box() : base(QBoxLayout::LeftToRight) {
  base::setSpacing(2);
  base::setMargin(2);
}

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

