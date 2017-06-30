// (qt_lib)

#pragma once

#include <dev_lib/inc/defs.hpp>

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

protected:
  grid();

  friend struct box;
  friend struct panel;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(box, QBoxLayout)
  box&   hb();
  box&   vb();
  grid&  gr();
  split& hs();
  split& vs();

  set_(margin,  (uint));
  set_(spacing, (uint));
  set_(add, (QWidget*));
  set_(addStretch, ());

protected:
  box();
dcl_end

dcl_sub_(hbox, box)
protected:
  hbox();

friend struct box;
  friend struct grid;
  friend struct panel;
dcl_end

dcl_sub_(vbox, box)
protected:
  vbox();

friend struct box;
  friend struct grid;
  friend struct panel;
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
