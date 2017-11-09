// (qt_lib)

#include "lst.hpp"
#include "../defs.inc"
#include "../wgt/font.hpp"
#include <QHeaderView>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPalette>

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
: isCheckable(false), isNumbered(false), state(triChk::eState::off) {
}

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
      check(rw_n(i), on, i+1 == iEnd);
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
    emit triStateChanged(state = newState);
}

void lst_model::fixColumns(lst_view& view) const {
  auto h = view.header();

  h->setMinimumSectionSize(16);
  h->resizeSections(QHeaderView::ResizeToContents);

  if (isCheckable) {
    int col = checkableCol();
    h->setSectionResizeMode(col, QHeaderView::Fixed);
    view.setColumnWidth(col, 8 + QCheckBox().sizeHint().rwidth());
  }

  // auto-sizing ourselves, Qt can't get to work
  auto&& colWdt = [this, &view](int col) -> int {
    int w = 0;
    auto&& m = view.fontMetrics();
    for_i_(rows()) {
      auto&& v = displayData(rw_n(i), col);
      auto&& s = v.isReal() ? QString::number(v.toDouble()) : v.toString();
      w = l::max(w, m.boundingRect(s).width());
    }
    return 24 + w; // TODO Qt views use what format?  (e.g. e+06)
  };

  if (isNumbered) {
    int col = numberedCol();
    h->setSectionResizeMode(col, QHeaderView::Fixed);
    view.setColumnWidth(col, colWdt(col));
  }

  for_i_(cols()) {
    int col = int(i + colOff());
    h->setSectionResizeMode(col, QHeaderView::Interactive);
    view.setColumnWidth(col, colWdt(col));
  }
}

void lst_model::signalReset() const {
  mut(*this).beginResetModel();
  mut(*this).endResetModel();
  updateTriState();
  emit columnsToFix();
}

void lst_model::signalRowChanged(rw_n rw) const {
  emit mut(*this).dataChanged(index(int(rw), 0), index(int(rw), columnCount() - 1));
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
      return displayData(rw, col);
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
    return displayData(rw, col);
  case Qt::TextAlignmentRole:
    return rightAlign(cl) ? Qt::AlignRight : Qt::AlignLeft;
  default:
    return QVariant();
  }
}

var lst_model::displayData(rw_n rw, int col) const {
  if (int(col) == numberedCol())
    return rw + 1;

  auto cl = lst_model::cl_n(l::to_u(col) - colOff());
  return cell(rw, cl);
}

uint lst_model::colOff() const {
  return 1 + (isCheckable ? 1 : 0) + (isNumbered ? 1 : 0);
}

//------------------------------------------------------------------------------

dcl_sub_(lst_view_itemDelegate, QStyledItemDelegate)
  using base::base;

  void paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const {
    painter->save();

    auto clr = option.widget->palette().color(QPalette::Midlight);
    painter->setPen(clr);
    auto&& r = option.rect;
    painter->drawLine(r.right(), r.top(), r.right(), r.bottom());

    painter->restore();
    base::paint(painter, option, index);
  }
dcl_end

//------------------------------------------------------------------------------

lst_view::lst_view(bool hasHeader_)
: hasHeader(hasHeader_), model(nullptr), currRow(-1) {
  base::setSelectionBehavior(SelectRows);
  base::setAlternatingRowColors(true);
  setItemDelegate(styleDelegeate.reset(new lst_view_itemDelegate));
  showHeader(hasHeader);

  connect(this, &Self::clicked, [this](rcIndex idx) {
    if (model && 1 == idx.column())
      checkRow(idx.row());
  });
}

lst_view::ref lst_view::showHeader(bool on) {
  base::header()->setVisible(mut(hasHeader) = on); RTHIS
}

lst_view::ref lst_view::setModel(lst_model const* model_) {
  if (model) {
    disconnect(con);
    base::setModel(nullptr);
    mut(model) = nullptr;
  }

  if (model_) {
    base::setModel(mutp(mut(model) = model_));
    con = model->onSigReset([this]() {
      fixColumns();
      if (!model->rows())
        onSelected(-1);
      setCurrentRow(currRow);
    });

    fixColumns();
    setCurrentRow(0);
  }

  RTHIS
}

lst_view::ref lst_view::checkRow(int row) {
  if (model && model->isCheckable && 0<=row && row <= int(model->rows()))
    mut(*model).check(rw_n(uint(row)));
  RTHIS
}

lst_view::ref lst_view::checkRows(int_vec::rc rows) {
  for (auto row : rows)
    checkRow(row);
  RTHIS
}

lst_view::rw_n_vec lst_view::checkedRows() const {
  rw_n_vec rws;
  if (model) {
    for_i_(model->rows())
      if (model->isChecked(rw_n(i)))
        rws.add(rw_n(i));
  }
  return rws;
}

int lst_view::currentRow() const {
  return currRow;
}

int lst_view::setCurrentRow(int row) {
  if (!model)
    return (currRow = -1);

  currRow = l::bound(0, row, int(model->rows()) - 1);
  base::setCurrentIndex(model->createIndex(currRow, 0));
  return currRow;
}

lst_view::ref lst_view::fixColumns() {
  base::hideColumn(0);  // should look like a list; 0th column is tree-like
  header()->setSectionsMovable(false);
  if (model)
    model->fixColumns(*this);
  RTHIS
}

lst_view::ref lst_view::fixColWidth(cl_n cl, int w) {
  setColWidth(cl, w);
  if (model)
    header()->setSectionResizeMode(int(model->colOff() + cl), QHeaderView::Fixed);
  RTHIS
}

lst_view::ref lst_view::setColWidth(cl_n cl, int w) {
  if (model)
    setColumnWidth(int(model->colOff() + cl), w);
  RTHIS
}

void lst_view::keyPressEvent(QKeyEvent* e) {
  if (!model)
    return;

  switch (e->key()) {
  case Qt::Key_Space:
    if (model)
      checkRow(currRow);
    break;
  case Qt::Key_Up:
    setCurrentRow(currRow - 1);
    break;
  case Qt::Key_Down:
    setCurrentRow(currRow + 1);
    break;
  case Qt::Key_PageUp:
    setCurrentRow(currRow - 10);  // TODO
    break;
  case Qt::Key_PageDown:
    setCurrentRow(currRow + 10);  // TODO
    break;
  case Qt::Key_Home:
    setCurrentRow(0);
    break;
  case Qt::Key_End:
    setCurrentRow(l::val_max(currRow));
    break;
  default:
    if (!onKey(e->key()))
      base::keyPressEvent(e);
  }
}

int lst_view::sizeHintForColumn(int) const {
  return width_m(*this, 1.6);
}

void lst_view::currentChanged(rcIndex current, rcIndex previous) {
  base::currentChanged(current, previous);
  currRow = current.row();
}

void lst_view::selectionChanged(QItemSelection const& selected,
                                QItemSelection const& deselected) {
  base::selectionChanged(selected, deselected);
  onSelected(currRow);
}

//------------------------------------------------------------------------------
}
// eof
