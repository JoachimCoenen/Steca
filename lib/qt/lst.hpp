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

  atr_(bool, hasHeader);
  set_(showHeader, (bool = true));

  ptr_(lst_model, model);
  set_(setModel,  (lst_model const*));

  set_(checkRow,  (int));
  set_(checkRows, (int_vec::rc));
  mth_(rw_n_vec,  checkedRows, ());

  mth_(int, currentRow, ()); // -1 if not
  mth_mut_(int, setCurrentRow, (int));

  set_(fixColumns,  ());

  set_(fixColWidth, (cl_n, int));
  set_(setColWidth, (cl_n, int));

  virtual voi_(onSelected, (int /*row*/)) {}
  virtual bol_(onKey,      (int /*key*/)) RET_(false)

private:
  void keyPressEvent(QKeyEvent*);
  int  sizeHintForColumn(int) const; // make narrow columns
  void currentChanged(QModelIndex const&, QModelIndex const&);
  void selectionChanged(QItemSelection const&, QItemSelection const&);

  int  currRow;
  QMetaObject::Connection con;
dcl_end

//------------------------------------------------------------------------------
}
// eof
