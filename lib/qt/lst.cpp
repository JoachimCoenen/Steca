// (qt_lib)

#include "lst.hpp"
#include "font.hpp"
#include "inc/defs.inc"
#include <QHeaderView>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPalette>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(lst_view_itemDelegate, QStyledItemDelegate)
  using base::base;

  void paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index ) const {
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
  setItemDelegate(new lst_view_itemDelegate);
  showHeader(hasHeader);

  connect(this, &Self::clicked, [this](QModelIndex const& idx) {
    if (model && 1 == idx.column())
      checkRow(idx.row());
  });
}

lst_view::ref lst_view::showHeader(bool on) {
  base::header()->setVisible((mut(hasHeader) = on)); RTHIS
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
  return mWidth(*this, 1.6);
}

void lst_view::currentChanged(QModelIndex const& current, QModelIndex const& previous) {
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
