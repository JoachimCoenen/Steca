// (lib/qt)

/**@file
 * A window splitter.
 */

#pragma once

#include "../defs.hpp"
#include <QSplitter>

namespace l_qt {
//------------------------------------------------------------------------------

struct win;
struct split;
struct box;
struct panel;

/// a splitter
dcl_sub_(split, QSplitter)
  split& hs();            ///< add a nested horizontal splitter
  split& vs();            ///< add a nested vertical splitter

  split& add(QWidget*);   ///< add a widget
  panel& addPanel();      ///< add a panel

protected:
  split();
dcl_end

/// a horizontal splitter
dcl_sub_(hSplit, split)
protected:
  hSplit();

  friend struct win;
  friend struct split;
  friend struct box;
dcl_end

/// a vertical splitter
dcl_sub_(vSplit, split)
protected:
  vSplit();

  friend struct win;
  friend struct split;
  friend struct box;
dcl_end

//------------------------------------------------------------------------------
}
// eof
