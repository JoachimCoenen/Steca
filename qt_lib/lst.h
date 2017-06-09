// (app_lib)

#ifndef APP_LIB_TYP_QT_LST_H
#define APP_LIB_TYP_QT_LST_H

#include "../inc/defs_h.h"
#include "model.h"
#include <QTreeView>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_view, QTreeView)
  lst_view();
 ~lst_view();

  set_(set, (lst_model*))

  atr_(bool, isCheckable)
  vir_set_(setCheckable, (bool = true))
  atr_(bool, hasHeader)
  vir_set_(showHeader, (bool = true))

  set_(selectRow, (lst_model::rw_n))
  mth_(int, selectedRow, ())

  void sizeColumns();

protected:
  lst_model *model;
  void keyPressEvent(QKeyEvent*);
  void checkRow(QModelIndex const&);
  void checkRow(int);
  // make narrow columns
  int sizeHintForColumn(int) const;
protected:
  QMetaObject::Connection mConn;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
