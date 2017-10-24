// (qt_lib)

#include "layout.hpp"
#include "inc/defs.inc"
#include "lbl.hpp"
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

grid::ref grid::addSection(strc tx, uint colSpan) {
  addWidget(new lbl(tx), rowCount(), 0, 1, int(colSpan));
  addHline(colSpan);
  RTHIS;
}

grid::ref grid::add(QWidget* wgt, uint row, uint col)
  SET_(addWidget(NEED_(wgt), int(row), int(col)))

grid::ref grid::add(strc tx, uint row, uint col)
  SET_(add(new lbl(tx), row, col))

grid::ref grid::add(l::vec<QWidget*> ws) {
  auto&& row = rowCount();
  for_i_(ws.size()) {
    auto&& w = ws.at(i);
    if (w)
      add(w, uint(row), i);
    QLabel* lbl;
    if ((lbl = dynamic_cast<QLabel*>(w))) {
      auto tx = lbl->text();
      if ("<" == tx.left(1))
        lbl->setText(tx.mid(1));
      else
        setAlignment(w, Qt::AlignRight);
    }
  }

  RTHIS;
}

grid::ref grid::addHline(uint colSpan) {
  auto&& line = new QFrame;
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  addWidget(line, rowCount(), 0, 1, int(colSpan));
  RTHIS;
}

grid::ref grid::addRowStretch()
  SET_(setRowStretch(rowCount(), 1))

grid::ref grid::addColStretch()
  SET_(setColumnStretch(columnCount(), 1))

grid::ref grid::addStretch() {
  addRowStretch(); addColStretch();
  RTHIS;
}

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

box::ref box::addSection(strc tx) {
  add(tx);
  addHline();
  RTHIS;
}

box::ref box::add(QWidget* wgt) {
  addWidget(NEED_(wgt)); RTHIS
}

box::ref box::add(QLayout* lay) {
   addLayout(NEED_(lay)); RTHIS
}

box::ref box::add(strc tx) {
  add(new lbl(tx)); RTHIS
}

box::ref box::addHline() {
  auto&& line = new QFrame;
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  add(line);

  RTHIS
}

box::ref box::addStretch()
  SET_(base::addStretch(1))

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

panel::panel() : l(nullptr) {}

box& panel::hb() {
  if (l)
    check_or_err_(dynamic_cast<hbox*>(l), "panel already has another layout");
  else
    setLayout(l = new hbox);
  return *static_cast<hbox*>(l);
}

box& panel::vb() {
  if (l)
    check_or_err_(dynamic_cast<vbox*>(l), "panel already has another layout");
  else
    setLayout(l = new vbox);
  return *static_cast<vbox*>(l);
}

grid& panel::gr() {
  if (l)
    check_or_err_(dynamic_cast<grid*>(l), "panel already has another layout");
  else
    setLayout(l = new grid);
  return *static_cast<grid*>(l);
}

//------------------------------------------------------------------------------

group::group(strc tx) : base(toQt(tx)), l(nullptr) {}

box& group::hb() {
  if (l)
    check_or_err_(dynamic_cast<hbox*>(l), "panel already has another layout");
  else
    setLayout(l = new hbox);
  return *static_cast<hbox*>(l);
}

box& group::vb() {
  if (l)
    check_or_err_(dynamic_cast<vbox*>(l), "panel already has another layout");
  else
    setLayout(l = new vbox);
  return *static_cast<vbox*>(l);
}

grid& group::gr() {
  if (l)
    check_or_err_(dynamic_cast<grid*>(l), "panel already has another layout");
  else
    setLayout(l = new grid);
  return *static_cast<grid*>(l);
}

//------------------------------------------------------------------------------
}
// eof

