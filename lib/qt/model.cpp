// (qt_lib)

#include "model.hpp"
#include "inc/defs.inc"
#include "font.hpp"
#include "lst.hpp"
#include <QHeaderView>

namespace l_qt {
//------------------------------------------------------------------------------

lst_model::triChk::triChk(strc s, lst_model& model_) : base(s), model(model_) {
  connect(this, &Self::stateChanged, [this](int state) {
    model.changeTriState(eState(state));
  });

  connect(&model, &lst_model::triStateChanged, [this](eState state) {
    set(state);
  });
}

//------------------------------------------------------------------------------

lst_model::lst_model()
: isCheckable(false), isNumbered(false), state(triChk::eState::off) {}

lst_model::ref lst_model::setCheckable(bool on) {
  mut(isCheckable) = on;
  signalReset();
  RTHIS
}

int lst_model::checkableCol() const {
  return isCheckable ? 1 : -1;
}

lst_model::triChk* lst_model::makeTriChk(strc s) {
  return new triChk(s, *this);
}

void lst_model::changeTriState(triChk::eState state) {
  using eState = triChk::eState;
  if (!rows()) {
    if (eState::off != state)
      emit triStateChanged(eState::off);
  } else if (eState::part != state) {
    bool on = eState::on == state;
    for_i_(rows())
      check(rw_n(i), on, true);
    signalReset();
  }
}

lst_model::ref lst_model::setNumbered(uint n) {
  mut(isNumbered) = n;
  signalReset();
  RTHIS
}

int lst_model::numberedCol() const {
  return isNumbered ? (isCheckable ? 2 : 1) : -1;
}

str lst_model::head(cl_n) const {
  return str::null;
}

var lst_model::cell(rw_n, cl_n) const {
  return var();
}

lst_model::ref lst_model::check(rw_n rw)
  SET_(check(rw, !isChecked(rw)))

lst_model::ref lst_model::check(rw_n, bool, bool)
  SET_()

bool lst_model::isChecked(rw_n) const {
  return false;
}

void lst_model::updateTriState() const {
  bool all = true, none = true;
  for_i_(rows())
    if (isChecked(rw_n(i)))
      none = false;
    else
      all = false;

  using eState = triChk::eState;
  eState newState = none ? eState::off : all ? eState::on : eState::part;
  if (state != newState)
    emit triStateChanged((state = newState));
}

void lst_model::fixColumns(lst_view& view) const {
  auto h = view.header();
  h->setMinimumSectionSize(oWidth(view, 1));

  if (isCheckable) {
    int col = checkableCol();
    view.setColumnWidth(col, mWidth(view, 1.6));
    h->setSectionResizeMode(col, QHeaderView::Fixed);
  }

  if (isNumbered) {
    int col = numberedCol();
    view.setColumnWidth(col, oWidth(view, 1 + isNumbered));
    h->setSectionResizeMode(col, QHeaderView::Fixed);
  }

  for_i_(cols())
    h->setSectionResizeMode(int(i + colOff()), QHeaderView::Interactive);
}

void lst_model::signalReset() const {
  mutp(this)->beginResetModel();
  mutp(this)->endResetModel();
  updateTriState();
}

int lst_model::columnCount(rcIndex) const {
  return int(cols() + colOff());
}

int lst_model::rowCount(rcIndex) const {
  return int(rows());
}

QVariant lst_model::headerData(int col, Qt::Orientation, int role) const {
  if (col <= 0 || col == checkableCol())
    return QVariant();

  if (col == numberedCol()) {
    switch (role) {
    case Qt::DisplayRole:
      return "#";
    case Qt::TextAlignmentRole:
      return Qt::AlignRight;
    default:
      return QVariant();
    }
  }

  EXPECT_(col >= int(colOff()))
  auto cl = lst_model::cl_n(l::to_u(col) - colOff());

  switch (role) {
  case Qt::DisplayRole:
    return toQt(head(cl));
  case Qt::TextAlignmentRole:
    return rightAlign(cl) ? Qt::AlignRight : Qt::AlignLeft;
  default:
    return QVariant();
  }
}

QVariant lst_model::data(rcIndex index, int role) const {
  int row = index.row(), rowCnt = rowCount(),
      col = index.column();
  if (row < 0 || rowCnt <= row || col < 1)
    return QVariant();

  auto rw = rw_n(l::to_u(row));

  if (col == checkableCol()) {
    switch (role) {
    case Qt::CheckStateRole:
      return isChecked(rw) ? Qt::Checked : Qt::Unchecked;
    default:
      return QVariant();
    }
  }

  if (col == numberedCol()) {
    switch (role) {
    case Qt::DisplayRole:
      return row + 1;
    case Qt::TextAlignmentRole:
      return Qt::AlignRight;
    default:
      return QVariant();
    }
  }

  EXPECT_(col >= int(colOff()))
  auto cl = lst_model::cl_n(l::to_u(col) - colOff());

  switch (role) {
  case Qt::DisplayRole:
    return cell(rw, cl);
  case Qt::TextAlignmentRole:
    return rightAlign(cl) ? Qt::AlignRight : Qt::AlignLeft;
  default:
    return QVariant();
  }
}

uint lst_model::colOff() const {
  return 1 + (isCheckable ? 1 : 0) + (isNumbered ? 1 : 0);
}

//------------------------------------------------------------------------------
}
// eof
