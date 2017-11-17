// (lib/qt)

/** @file
 * a standard list view and the associated data model; single-selection;
 * note: internally, a Qt tree view is used, because it looks better
 * and provides more functionality
 */

#pragma once

#include "../defs.hpp"
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

/** an abstracted (simplified from Qt)  list model
 */
dcl_sub_(lst_model, QAbstractTableModel)
  /// a type-safe(r) column number
  use_int_(cl_n, uint, {})
  /// a type-safe(r) row number
  use_int_(rw_n, uint, {})
  /// a vector of (z.B. selected) row numbers
  using rw_n_vec = uint_vec;

  lst_model();

  /// a checkable model adds automatically a column of checkboxes
  atr_(bool, isCheckable);
  set_(setCheckable, (bool = true));

private:
  /// the index of the column of checkboxes, or -1 if none
  mth_(int, checkableCol, ());

public:
  /// a tri-state checkbox attached to the model (for "select none / some / all")
  dcl_sub_(triChk, l_qt::triChk)
    triChk(strc, lst_model&);
  private:
    lst_model& model;
  dcl_end

  /// make an instance of that tri-state checkbox
  mth_mut_(triChk*, makeTriChk, (strc));
  /// change the tri-state ("select none / some / all")
  mut_(changeTriState, (triChk::eState));

  /// "numbered" adds automatic column with row numbers
  atr_(uint, isNumbered);
  set_(setNumbered, (uint));

private:
  /// the index of the column with row numbers, or -1 if none
  mth_(int, numberedCol, ());

public:
  /// specify the number of ordinary columns (not including checkboxes or numbers)
  virtual mth_(cl_n, cols, ()) = 0;
  /// specify the number of rows
  virtual mth_(rw_n, rows, ()) = 0;

  /// specify the column headers
  virtual mth_(str,  head, (cl_n));
  /// specify the cell values
  virtual mth_(var,  cell, (rw_n, cl_n));

  /// if @c isCheckable, flip the check state of a row
  virtual set_(check, (rw_n));
  /** if @c isCheckable, set the check state of a row;
   * if there is a generated sequence of @c check (by @c changeTriState), all
   * but the last @c check call will have <code>done = false</code>
   */
  virtual set_(check, (rw_n, bool, bool done = true));
  /// if @c isCheckable, obtain the check state of a row
  virtual bol_(isChecked, (rw_n));

private:
  /// updates the tri-state (none / some /all) and emits a signal
  voi_(updateTriState, ()) emits;

public:
  /// nicely resizes columns in a given view
  voi_(fixColumns,  (lst_view&));
  /// tell which columns ought to be right-aligned
  virtual bol_(rightAlign, (cl_n)) RET_(false)

signals:
  /// a new tri-state
  void triStateChanged(triChk::eState) const;

public:
  /// a helper to connect to the reset signal
  template <typename Lambda>
  QMetaObject::Connection onSigReset(Lambda slot) const {
    return QObject::connect(this, &Self::modelReset, slot);
  }

protected:
  /// emits the reset signal
  voi_(signalReset, ())           emits;
  /// emits the dataChanged signal - notifies that a row has changed
  voi_(signalRowChanged, (rw_n))  emits;

private:
  mutable triChk::eState state;

protected:
  // implemented Qt methods
  using Index   = QModelIndex;
  using rcIndex = Index const&;

  int columnCount(rcIndex = Index()) const;
  int rowCount(rcIndex = Index())    const;

  QVariant headerData(int, Qt::Orientation, int) const;
  QVariant data(rcIndex, int)                    const;
  var displayData(rw_n, int col) const;

  // column offset (counting for optional checkboxes and row numbers)
  uint colOff() const;
  friend struct lst_view;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

/** an abstracted (simplified from Qt) list view
 */
dcl_sub_(lst_view, QTreeView)
  lst_view(bool hasHeader = true);

  using cl_n     = lst_model::cl_n;
  using rw_n     = lst_model::rw_n;
  using rw_n_vec = lst_model::rw_n_vec;

protected:
  using Index    = lst_model::Index;
  using rcIndex  = lst_model::rcIndex;

public:
  /// show the header ?
  atr_(bool, hasHeader);
  /// show the header ?
  set_(showHeader, (bool = true));

  /// the associated model
  ptr_(lst_model, model);
  /// set the associated model
  set_(setModel,  (lst_model const*));

  /// flip the check state of a row
  set_(checkRow,  (int));
  /// flip the check state of a set of rows
  set_(checkRows, (int_vec::rc));
  /// obtain the vector of checked rows
  mth_(rw_n_vec,  checkedRows, ());

  /// the current (selected) row, or -1
  mth_(int, currentRow, ());

  /// set the current (selected) row
  mth_mut_(int, setCurrentRow, (int));

  /// nicely resize columns
  set_(fixColumns,  ());

  /// resize and fix a column
  set_(fixColWidth, (cl_n, int width));
  /// resize a column
  set_(setColWidth, (cl_n, int width));

  /// a callback if selection changes
  virtual voi_(onSelected, (int /*row*/)) {}
  /// a callback if a key is pressed; return @c true if keypress is handled
  virtual bol_(onKey,      (int /*key*/)) RET_(false)

private:
  // implemented Qt methods
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
