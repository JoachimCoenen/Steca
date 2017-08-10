// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
#include <dev_lib/typ/vec.hpp>
#include "model.hpp"

#include <QTreeView>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_view, QTreeView)
  lst_view();

  using cl_n     = lst_model::cl_n;
  using rw_n     = lst_model::rw_n;
  using cl_n_vec = lst_model::cl_n_vec;
  using rw_n_vec = lst_model::rw_n_vec;

  atr_(bool, hasHeader);
  set_(showHeader, (bool = true));

  ptr_(lst_model, model);
  set_(setModel, (lst_model const*));

  set_(checkRow,  (QModelIndex const&));
  set_(checkRow,  (rw_n));
  set_(checkRows, (rw_n_vec::rc));
  mth_(rw_n_vec,  checkedRows, ());

  mth_(int, currentRow,  ()); // -1 if not

  set_(selectRow, (rw_n));
  mth_(int, selectedRow, ()); // -1 if not

  set_(selectRows,(rw_n_vec::rc));
  mth_(rw_n_vec,   selectedRows, ());

  set_(sizeColumns, ());

protected:
  void keyPressEvent(QKeyEvent*);
  int  sizeHintForColumn(int) const; // make narrow columns
  QMetaObject::Connection con;
dcl_end

//------------------------------------------------------------------------------
}
// eof
