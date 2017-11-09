// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <lib/dev/typ/vec.hpp>
#include <QBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QGroupBox>

namespace l_qt {
//------------------------------------------------------------------------------

struct box;
struct panel;
struct split;

dcl_sub_(grid, QGridLayout)
  box&  hb(uint row, uint col);
  box&  vb(uint row, uint col);
  grid& gr(uint row, uint col);

  set_(addSection, (strc, uint colSpan));
  set_(add, (QWidget*, uint row, uint col));
  set_(add, (strc,     uint row, uint col));
  set_(add, (l::vec<QWidget*>));
  set_(addHline, (uint colSpan));

  set_(addRowStretch, ());
  set_(addColStretch, ());
  set_(addStretch,    ());

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

  set_(addSection, (strc));
  set_(add,     (QWidget*));
  set_(add,     (QLayout*));
  set_(add,     (strc));
  set_(addHline, ());

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

private:
  QLayout *l;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(group, QGroupBox)
  group(strc tx);

  box&  hb() may_err;
  box&  vb() may_err;
  grid& gr() may_err;

private:
  QLayout *l;
dcl_end

//------------------------------------------------------------------------------
}
// eof
