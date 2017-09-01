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

lst_view::lst_view(bool hasHeader_) : hasHeader(hasHeader_), model(nullptr) {
  base::setSelectionBehavior(SelectRows);
  base::setAlternatingRowColors(true);
  setItemDelegate(new lst_view_itemDelegate);
  showHeader(hasHeader);

  connect(this, &Self::clicked, [this](idx_rc idx) {
    if (model && model->isCheckable && 1 == idx.column())
      checkRow(base::currentIndex());
  });
}

lst_view::ref lst_view::showHeader(bool on) {
  base::header()->setVisible((mut(hasHeader) = on)); RTHIS
}

lst_view::ref lst_view::setModel(lst_model const* model_) {
  if (model) {
    disconnect(modelConn);
    base::setModel(nullptr);
    mut(model) = nullptr;
    base::header()->setVisible(false);
  }

  if (model_) {
    base::setModel(mutp(mut(model) = model_));
    fixColumns();
    selectRow(rw_n(0));
  }

  RTHIS
}

lst_view::ref lst_view::checkRow(idx_rc index) {
  auto row = index.row();
  if (row >= 0)
    checkRow(rw_n(l::to_uint(row)));
  RTHIS
}

lst_view::ref lst_view::checkRow(rw_n rw) {
  if (model) {
    EXPECT_(model->rows())
    mutp(model)->check(rw);
    QModelIndex index = model->index(int(rw), 1);
    emit dataChanged(index, index);
  }
  RTHIS
}

lst_view::ref lst_view::checkRows(rw_n_vec::rc rws) {
  for (auto rw : rws)
    checkRow(rw);
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
  return base::currentIndex().row();
}

lst_view::ref lst_view::selectRow(rw_n rw) {
  if (model)
    base::setCurrentIndex(model->index(int(rw), 1));
  RTHIS
}

int lst_view::selectedRow() const {
  auto rws = selectedRows();
  return rws.isEmpty() ? -1 : int(rws.first());
}

lst_view::ref lst_view::selectRows(rw_n_vec::rc rws) {
  if (model) {
    int cols = model->columnCount();

    QItemSelection is;
    for (auto rw : rws)
      is.append(QItemSelectionRange(model->index(int(rw), 0),
                                    model->index(int(rw), cols - 1)));
    selectionModel()->select(is, QItemSelectionModel::ClearAndSelect);
  }
  RTHIS
}

lst_view::rw_n_vec lst_view::selectedRows() const {
  rw_n_vec rws;
  for (auto&& index : selectionModel()->selectedRows())
    rws.add(rw_n(l::to_u(index.row())));
  return rws;
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
  switch (e->key()) {
  case Qt::Key_Space:
    if (model && model->isCheckable)
      checkRow(base::currentIndex());
    return;
  default:
    base::keyPressEvent(e);
  }
}

int lst_view::sizeHintForColumn(int) const {
  return mWidth(*this, 1.6);
}

//------------------------------------------------------------------------------
}
// eof
