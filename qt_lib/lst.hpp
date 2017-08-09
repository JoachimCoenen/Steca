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

  using cl_n = lst_model::cl_n;
  using rw_n = lst_model::rw_n;

  ptr_(lst_model, model);
  set_(setModel, (lst_model const*));

  atr_(bool, isCheckable);
  set_(setCheckable, (bool = true));

  atr_(bool, hasHeader);
  set_(showHeader,   (bool = true));

  mth_(int, currentRow,  ()); // -1 if not

  set_(selectRow, (rw_n));
  mth_(int, selectedRow, ()); // -1 if not

  set_(selectRows, (l::vec<rw_n>));
  mth_(l::vec<rw_n>, selectedRows, ());

  set_(sizeColumns, ());

protected:
  void keyPressEvent(QKeyEvent*);
  void checkRow(QModelIndex const&);
  void checkRow(int);

  int sizeHintForColumn(int) const; // make narrow columns
  QMetaObject::Connection con;
dcl_end

//------------------------------------------------------------------------------
}
// eof
