// (app_lib)

#ifndef APP_LIB_TYP_QT_MODEL_H
#define APP_LIB_TYP_QT_MODEL_H

#include "../inc/defs_h.h"
#include "../inc/num.h"
#include "var.h"
#include <QAbstractTableModel>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_model, QAbstractTableModel)
  use_num_ (cl_n, uint, {}) // col number
  use_num_ (rw_n, uint, {}) // row number

  lst_model();

  atr_(bool, isCheckable)
  vir_set_(checkable, (bool))

  abs_mth_(cl_n, cols, ())
  abs_mth_(rw_n, rows, ())
  vir_mth_(str,  head, (cl_n))
  abs_mth_(var,  cell, (rw_n, cl_n))

  vir_set_(check, (rw_n))
  vir_set_(check, (rw_n, bool))
  vir_mth_(bool, isChecked, (rw_n))

  mth_(void, notify, ())

protected:
  using rcIndex = QModelIndex const&;
  int columnCount(rcIndex = QModelIndex())       const;
  int rowCount(rcIndex = QModelIndex())          const;
  QVariant headerData(int, Qt::Orientation, int) const;
  QVariant data(rcIndex, int)                    const;

  uint colOff() const;
  friend struct lst_view;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
