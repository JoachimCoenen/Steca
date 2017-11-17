// (lib/qt)

/**@file
 * Make layouts of widgets.
 */

#pragma once

#include "../defs.hpp"
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

/// a wrapped Qt grid layout
dcl_sub_(grid, QGridLayout)
  box&  hb(uint row, uint col); ///< add a nested horizontal box at row-col
  box&  vb(uint row, uint col); ///< add a nested vertical box at row-col
  grid& gr(uint row, uint col); ///< add a nested grid at row-col

  set_(addSection, (strc, uint colSpan));     ///< add a label and a horizontal line at the bottom
  set_(add, (QWidget*, uint row, uint col));  ///< add a widget at row-col
  set_(add, (strc,     uint row, uint col));  ///< add a label at row-col
  set_(add, (l::vec<QWidget*>));              ///< add a full row of widgets at the bottom
  set_(addHline, (uint colSpan));             ///< add a horitontal line at the bottom

  set_(addRowStretch, ());      ///< add a row stretch at the bottom
  set_(addColStretch, ());      ///< add a column stretch at the right side
  set_(addStretch,    ());      ///< add a stretch at the bottom and the right side

  grid();
dcl_end

//------------------------------------------------------------------------------

/// a wrapped Qt box layout
dcl_sub_(box, QBoxLayout)
  box&   hb();  ///< add a nested horizontal box
  box&   vb();  ///< add a nested vertical box
  grid&  gr();  ///< add a nested grid
  split& hs();  ///< add a nested horizontal split
  split& vs();  ///< add a nested vertical split

  box();

  set_(margin,  (uint));      ///< set the margin
  set_(spacing, (uint));      ///< set the element spacing

  set_(addSection, (strc));   ///< add a label and a horizontal line
  set_(add,     (QWidget*));  ///< add a widget
  set_(add,     (QLayout*));  ///< add a layout
  set_(add,     (strc));      ///< add a horitzontal line
  set_(addHline, ());

  set_(addStretch, ());       ///< add a stretch

  set_(align,   (QLayout&, Qt::Alignment));   ///< set an element's alignment
  set_(align,   (QWidget*, Qt::Alignment));   ///< set an element's alignment
dcl_end

dcl_sub_(hbox, box)
  hbox();
dcl_end

dcl_sub_(vbox, box)
  vbox();
dcl_end

//------------------------------------------------------------------------------

/// a panel - a widget with a layout
dcl_sub_(panel, QWidget)
  panel();

  box&  hb();   ///< make a horizontal box the main layout of the widget
  box&  vb();   ///< make a vertical box the main layout of the widget
  grid& gr();   ///< make a grid the main layout of the widget

private:
  QLayout *l;
dcl_end

//------------------------------------------------------------------------------

/// a group - a framed widget with a layout
dcl_sub_(group, QGroupBox)
  group(strc label);

  box&  hb();   ///< make a horizontal box the main layout of the widget
  box&  vb();   ///< make a vertical box the main layout of the widget
  grid& gr();   ///< make a grid the main layout of the widget

private:
  QLayout *l;
dcl_end

//------------------------------------------------------------------------------
}
// eof
