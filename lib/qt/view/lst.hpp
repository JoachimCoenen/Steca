// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <lib/dev/inc/num.hpp>
#include <lib/dev/inc/ptr.hpp>
#include <lib/dev/inc/vecs.hpp>
#include "../wgt/chk.hpp"
#include "../app/var.hpp"
#include <QAbstractTableModel>
#include <QTreeView>

namespace l_qt {
//------------------------------------------------------------------------------

struct lst_view;

dcl_sub_(lst_model, QAbstractTableModel)
  use_int_(cl_n, uint, {}) // col number
  use_int_(rw_n, uint, {}) // row number

  use_typ_(cl_n_vec, uint_vec)
  use_typ_(rw_n_vec, uint_vec)

  lst_model();

  atr_(bool, isCheckable);
  set_(setCheckable, (bool = true));
  mth_(int, checkableCol, ());

  dcl_sub_(triChk, l_qt::triChk)
    triChk(strc, lst_model&);
  private:
    lst_model& model;
  dcl_end

  mth_mut_(triChk*, makeTriChk, (strc));
  mut_(changeTriState, (triChk::eState));

  atr_(uint, isNumbered);
  set_(setNumbered, (uint));
  mth_(int, numberedCol, ());

  virtual mth_(cl_n, cols, ()) = 0;
  virtual mth_(rw_n, rows, ()) = 0;

  virtual mth_(str,  head, (cl_n));
  virtual mth_(var,  cell, (rw_n, cl_n));

  virtual set_(check, (rw_n));
  virtual set_(check, (rw_n, bool, bool done = true));
  virtual bol_(isChecked, (rw_n));

  voi_(updateTriState, ()) emits;

  voi_(fixColumns,  (lst_view&));
  virtual bol_(rightAlign, (cl_n)) RET_(false)

signals:
  void triStateChanged(triChk::eState) const;
  void columnsToFix() const;

public:
  template <typename Lambda>
  QMetaObject::Connection onSigReset(Lambda slot) const {
    return QObject::connect(this, &Self::modelReset, slot);
  }

protected:
  voi_(signalReset, ())           emits;
  voi_(signalRowChanged, (rw_n))  emits;

private:
  mutable triChk::eState state;

protected:
  using Index   = QModelIndex;
  using rcIndex = Index const&;

  int columnCount(rcIndex = Index()) const;
  int rowCount(rcIndex = Index())    const;

  QVariant headerData(int, Qt::Orientation, int) const;
  QVariant data(rcIndex, int)                    const;
  var displayData(rw_n, int col) const;

  uint colOff() const;
  friend struct lst_view;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
dcl_sub_(lst_view, QTreeView)
  lst_view(bool hasHeader = true);

  using cl_n     = lst_model::cl_n;
  using rw_n     = lst_model::rw_n;
  using cl_n_vec = lst_model::cl_n_vec;
  using rw_n_vec = lst_model::rw_n_vec;

  using Index    = lst_model::Index;
  using rcIndex  = lst_model::rcIndex;

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
  void currentChanged(rcIndex, rcIndex);
  void selectionChanged(QItemSelection const&, QItemSelection const&);

  int  currRow;
  QMetaObject::Connection con;
  l::scoped<QAbstractItemDelegate> styleDelegeate;
dcl_end

//------------------------------------------------------------------------------
}
// eof
