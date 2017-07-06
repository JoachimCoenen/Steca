// (qt_lib)

#include "model.hpp"
#include <dev_lib/defs.inc>

#include "str_inc.hpp"

#include <QSize>

namespace l_qt {
//------------------------------------------------------------------------------

lst_model::lst_model() : isCheckable(false) {}

lst_model::ref lst_model::setCheckable(bool on) SET_(mut(isCheckable) = on)

str lst_model::head(cl_n) const {
  return str::null;
}

var lst_model::cell(rw_n, cl_n) const {
  return var();
}

lst_model::ref lst_model::check(rw_n rw) SET_(check(rw, !isChecked(rw)))

lst_model::ref lst_model::check(rw_n, bool) SET_()

bool lst_model::isChecked(rw_n) const {
  return false;
}

void lst_model::notify() const {
  mutp(this)->beginResetModel();
  mutp(this)->endResetModel();
}

int lst_model::columnCount(rcIndex) const {
  return int(cols() + colOff());
}

int lst_model::rowCount(rcIndex) const {
  return int(rows());
}

QVariant lst_model::headerData(int col, Qt::Orientation, int role) const {
  if (col < int(colOff()))
    return QVariant();

  auto cl = lst_model::cl_n(l::to_u(col) - colOff());

  switch (role) {
  case Qt::DisplayRole:
    return toQt(head(cl));
  default:
    return QVariant();
  }
}

QVariant lst_model::data(rcIndex index, int role) const {
  int row = index.row(), rowCnt = rowCount(),
      col = index.column();
  if (row < 0 || rowCnt <= row || col < 1)
    return QVariant();

  auto rw = lst_model::rw_n(l::to_u(row));

  if (isCheckable && 1==col) {
    switch (role) {
    case Qt::SizeHintRole:
      return QSize(4, 0);
    case Qt::CheckStateRole:
      return isChecked(rw) ? Qt::Checked : Qt::Unchecked;
    default:
      return QVariant();
    }
  }

  EXPECT_(col >= int(colOff()))
  auto cl = lst_model::cl_n(l::to_u(col) - colOff());

  switch (role) {
  case Qt::DisplayRole: {
    return cell(rw, cl);
  }
  default:
    return QVariant();
  }
}

uint lst_model::colOff() const {
  return isCheckable ? 2 : 1;
}

//------------------------------------------------------------------------------
}
// eof
