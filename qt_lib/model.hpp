// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
#include <dev_lib/inc/num.hpp>
#include "var.hpp"
#include <QAbstractTableModel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_model, QAbstractTableModel)
  use_num_(cl_n, uint, {}) // col number
  use_num_(rw_n, uint, {}) // row number

  lst_model();

  atr_(bool, isCheckable);
  set_(setCheckable, (bool));

  virtual mth_(cl_n, cols, ()) = 0;
  virtual mth_(rw_n, rows, ()) = 0;

  virtual mth_(str,  head, (cl_n));
  virtual mth_(var,  cell, (rw_n, cl_n));

  virtual set_(check, (rw_n));
  virtual set_(check, (rw_n, bool));
  virtual mth_(bool, isChecked, (rw_n));

  mth_(void, notify, ());

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
