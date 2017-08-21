// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <lib/dev/inc/num.hpp>
#include <lib/dev/inc/vecs.hpp>
#include "var.hpp"
#include <QAbstractTableModel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_model, QAbstractTableModel)
  use_int_(cl_n, uint, {}) // col number
  use_int_(rw_n, uint, {}) // row number

  use_vec_(cl_n, uint)
  use_vec_(rw_n, uint)

  lst_model();

  atr_(bool, isCheckable);
  set_(setCheckable, (bool = true));
  mth_(int, checkableCol, ());

  atr_(uint, isNumbered);
  set_(setNumbered, (uint));
  mth_(int, numberedCol, ());

  virtual mth_(cl_n, cols, ()) = 0;
  virtual mth_(rw_n, rows, ()) = 0;

  virtual mth_(str,  head, (cl_n));
  virtual mth_(var,  cell, (rw_n, cl_n));

  virtual set_(check, (rw_n));
  virtual set_(check, (rw_n, bool));
  virtual bol_(isChecked, (rw_n));

  act_(signalReset, ());

protected:
  using Index   = QModelIndex;
  using rcIndex = Index const&;

  int columnCount(rcIndex = Index()) const;
  int rowCount(rcIndex = Index())    const;

  QVariant headerData(int, Qt::Orientation, int) const;
  QVariant data(rcIndex, int)                    const;

  uint colOff() const;
  friend struct lst_view;
dcl_end

//------------------------------------------------------------------------------
}
// eof
