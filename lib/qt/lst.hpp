// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include "model.hpp"
#include <QTreeView>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_view, QTreeView)
  lst_view(bool hasHeader = true);

  using cl_n     = lst_model::cl_n;
  using rw_n     = lst_model::rw_n;
  using cl_n_vec = lst_model::cl_n_vec;
  using rw_n_vec = lst_model::rw_n_vec;

  using idx_rc   = QModelIndex const&;

  atr_(bool, hasHeader);
  set_(showHeader, (bool = true));

  ptr_(lst_model, model);
  set_(setModel,  (lst_model const*));

  set_(checkRow,  (idx_rc));
  set_(checkRow,  (rw_n));
  set_(checkRows, (rw_n_vec::rc));
  mth_(rw_n_vec,  checkedRows, ());

  mth_(int, currentRow,  ()); // -1 if not

  set_(selectRow, (rw_n));
  mth_(int, selectedRow, ());             // -1 if not

  set_(selectRows,(rw_n_vec::rc));
  mth_(rw_n_vec, selectedRows, ());

  virtual set_(fixColumns,  ());

  set_(fixColWidth, (cl_n, int));
  set_(setColWidth, (cl_n, int));

protected:
  void keyPressEvent(QKeyEvent*);
  int  sizeHintForColumn(int) const; // make narrow columns
  void selectionChanged(QItemSelection const&, QItemSelection const&);
  int  selRow;
dcl_end

//------------------------------------------------------------------------------
}
// eof
