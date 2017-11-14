// (qt_lib)

#include "table.hpp"
#include "../defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

ModelTable::numRow::numRow(uint n_, row_t::rc row_) : n(n_), row(row_) {}

ModelTable::ModelTable(uint numCols_)
: headData(), numCols(numCols_), sortCol(-1) {
  colIndexMap.resize(numCols);
  for_i_(numCols)
    colIndexMap.setAt(i, i);
}

str ModelTable::head(cl_n cl) const {
  if (0 == cl)
    return "#";

  EXPECT_(0 < cl)
  return headData.at(--cl);
}

var ModelTable::cell(rw_n rw, cl_n cl) const {
  if (0 == cl)
    return rowsData.at(rw)->n;

  auto&& val = rowsData.at(rw)->row.at(--cl);
  if (val.isReal() && l::isnan(val.toReal()))
    return var(); // hide NaNs

  return val;
}

bool ModelTable::rightAlign(cl_n cl) const {
  if (0 == cl)
    return true;

  auto&& fun = cmpFuns.at(--cl);
  return var::cmp_int == fun || var::cmp_real == fun;
}

void ModelTable::clear() {
  rowsData.clear();
  signalReset();
}

ModelTable::ref ModelTable::add(ModelTable::row_t::rc row, bool sort) {
  rowsData.add(new numRow(rowsData.size() + 1, row));
  if (sort)
    sortData();
  RTHIS;
}

void ModelTable::setCols(str_vec::rc headData_, cmp_vec::rc cmpFuns_) {
  EXPECT_(headData.size() == numCols && cmpFuns.size() == numCols)
  headData = headData_;
  cmpFuns  = cmpFuns_;
}

void ModelTable::setSortCol(int col) {
  sortCol = col < 0 ? col : int(colIndexMap.at(uint(col)));
}

void ModelTable::sortByCol(int srtCol, Qt::SortOrder order) {
  auto cmpRows = [this, order](uint col, row_t::rc r1, row_t::rc r2) {
    col = colIndexMap.at(col);
    auto&& res = cmpFuns.at(col)(r1.at(col), r2.at(col));
    if (Qt::DescendingOrder == order)
      res *= -1;
    return res;
  };

  // sort by sortColumn first, then left-to-right
  auto cmp = [this, cmpRows, srtCol](numRow const* r1, numRow const* r2) {
    if (0 <= srtCol) {
      int c = cmpRows(l::to_u(srtCol), r1->row, r2->row);
      if (c < 0)
        return true;
      if (c > 0)
        return false;
    } else if (-1 == sortCol) {
      if (r1->n < r2->n)
        return true;
      if (r1->n > r2->n)
        return false;
    }

    for_var_(col, cols()) {
      if (int(col) != srtCol) {
        int c = cmpRows(col, r1->row, r2->row);
        if (c < 0)
          return true;
        if (c > 0)
          return false;
      }
    }

    return false;
  };

  beginResetModel();
  std::stable_sort(rowsData.begin(), rowsData.end(), cmp);
  endResetModel();
}

void ModelTable::sortData() {
  sortByCol(sortCol);
}

void ModelTable::moveColumn(uint from, uint to) {
  colIndexMap.swap(from, to);
}

//------------------------------------------------------------------------------
}
// eof DOCS
