// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <QBoxLayout>
#include <QGridLayout>
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

struct box;
struct panel;
struct split;

dcl_sub_(grid, QGridLayout)
  box&  hb(uint row, uint col);
  box&  vb(uint row, uint col);
  grid& gr(uint row, uint col);

  set_(add, (QWidget*, uint row, uint col));
  set_(addRowStretch, ());
  set_(addColStretch, ());

  grid();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(box, QBoxLayout)
  box&   hb();
  box&   vb();
  grid&  gr();
  split& hs();
  split& vs();

  box();

  set_(margin,  (uint));
  set_(spacing, (uint));
  set_(add, (QWidget*));
  set_(addStretch, ());
  set_(align,   (QLayout&, Qt::Alignment));
  set_(align,   (QWidget*, Qt::Alignment));
dcl_end

dcl_sub_(hbox, box)
  hbox();
dcl_end

dcl_sub_(vbox, box)
  vbox();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(panel, QWidget)
  panel();

  box&  hb() may_err;
  box&  vb() may_err;
  grid& gr() may_err;
dcl_end

//------------------------------------------------------------------------------
}
// eof
