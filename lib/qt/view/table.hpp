// (lib/qt)

/** @file
 * a standard table view and the associated data model
 */

#pragma once

#include "../view/lst.hpp"
#include "../app/var.hpp"
#include <lib/dev/typ/vec.hpp>

namespace l_qt {
//------------------------------------------------------------------------------

/// a table model implementation
dcl_sub_(ModelTable, lst_model)
  /// a row is a vec of variant
  using row_t   = l::vec<var>;
  /// a corresponding vec of 3-way compare functions
  using cmp_vec = l::vec<var::cmpFun*>;

  /// a row with added stored row number
  dcl_(numRow)
    atr_(uint,  n);   ///< the row number
    atr_(row_t, row); ///< the rest of the row
    numRow(uint, row_t::rc);
  dcl_end

  /// a table with a given number of columns
  ModelTable(uint numCols);

  /// implemented @c cols from base class
  mth_(cl_n, cols, ()) RET_(cl_n(numCols))
  /// implemented @c rows from base class
  mth_(rw_n, rows, ()) RET_(rw_n(rowsData.size()))

  /// implemented @c head from base class
  mth_(str,  head, (cl_n));
  /// implemented @c cell from base class
  mth_(var,  cell, (rw_n, cl_n));
  /// implemented @c rightAlign from base class
  bol_(rightAlign, (cl_n));

  /// empty the table
  mut_(clear, ());

  /// set the head and compare functions
  mut_(setCols,    (str_vec::rc headData, cmp_vec::rc cmpFuns));
  /// set the column to sort by
  mut_(setSortCol, (int col));

  /// add a row
  set_(add, (ModelTable::row_t::rc, bool sort = true));

  /// sort by a column
  mut_(sortByCol, (int col, Qt::SortOrder = Qt::AscendingOrder));
  /// sort the whole table
  mut_(sortData, ());

  /// move a table column
  mut_(moveColumn, (uint from, uint to));

protected:
  str_vec                   headData;
  l::vec<l::scoped<numRow>> rowsData;

  uint     numCols;
  int      sortCol;
  uint_vec colIndexMap;
  cmp_vec  cmpFuns;
dcl_end

//------------------------------------------------------------------------------

/// this is simple
dcl_sub_(Table, lst_view)
  Table();
dcl_end

//------------------------------------------------------------------------------
}
// eof
