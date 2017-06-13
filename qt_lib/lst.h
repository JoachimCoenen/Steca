// (qt_lib)

#ifndef QT_LIB_LST_H
#define QT_LIB_LST_H

#include <dev_lib/inc/defs_h.h>
#include "model.h"
#include <QTreeView>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_view, QTreeView)
  lst_view();

  ptr_(lst_model, model);
  set_(set, (lst_model*));

  atr_(bool, isCheckable);
  set_(setCheckable, (bool = true));

  atr_(bool, hasHeader);
  set_(showHeader,   (bool = true));

  set_(selectRow, (lst_model::rw_n));
  mth_(int, selectedRow, ());

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
#endif
// eof
