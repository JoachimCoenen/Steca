// (qt_lib)

#pragma once
#include "../view/lst.hpp"
#include "../app/var.hpp"
#include <lib/dev/typ/vec.hpp>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(ModelTable, lst_model)
  using row_t = l::vec<var>;
  using cmp_vec = l::vec<var::cmpFun*>;

  dcl_(numRow)
    atr_(uint,  n);   // row numbers (as added to rowsData)
    atr_(row_t, row); // row cell values
    numRow(uint, row_t::rc);
  dcl_end

  ModelTable(uint numCols);

  mth_(cl_n, cols, ()) RET_(cl_n(numCols))
  mth_(rw_n, rows, ()) RET_(rw_n(rowsData.size()))

  mth_(str,  head, (cl_n));
  mth_(var,  cell, (rw_n, cl_n));
  bol_(rightAlign, (cl_n));

  mut_(clear, ());

  mut_(setCols,    (str_vec::rc headData, cmp_vec::rc cmpFuns));
  mut_(setSortCol, (int col));

  set_(add, (ModelTable::row_t::rc, bool sort = true));

  mut_(sortByCol, (int col, Qt::SortOrder = Qt::AscendingOrder));
  mut_(sortData, ());

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

dcl_sub_(Table, lst_view)
  Table();
dcl_end

//------------------------------------------------------------------------------
}
// eof
