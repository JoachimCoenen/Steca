// (qt_lib)

#include "lst.hpp"
#include "font.hpp"
#include <dev_lib/defs.inc>

#include <QHeaderView>
#include <QKeyEvent>

namespace l_qt {
//------------------------------------------------------------------------------

lst_view::lst_view() : hasHeader(false), model(nullptr) {
  base::setSelectionBehavior(SelectRows);
  base::setAlternatingRowColors(true);
  showHeader(hasHeader);

  connect(this, &Self::clicked, [this](QModelIndex const& index) {
    if (model && model->isCheckable && 1 == index.column())
      checkRow(base::currentIndex());
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
    base::header()->setVisible(false);
  }

  if (model_) {
    base::setModel(mutp(mut(model) = model_));
    sizeColumns();
    selectRow(rw_n(0));

    con = connect(model, &QAbstractTableModel::modelReset, [this]() {
      sizeColumns();
    });
  }

  RTHIS
}

lst_view::ref lst_view::checkRow(QModelIndex const& index) {
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
  for (auto& index : selectionModel()->selectedRows())
    rws.add(rw_n(l::to_u(index.row())));
  return rws;
}

lst_view::ref lst_view::sizeColumns() {
  if (model) {
    base::hideColumn(0);  // should look like a list; 0th column is tree-like

    for_i_(int(model->cols() + model->colOff()))
      base::header()->setSectionResizeMode(i, QHeaderView::Interactive);

    if (model->isCheckable) {
      int col = model->checkableCol();
      base::setColumnWidth(col, mWidth(this, 1.6));
      base::header()->setSectionResizeMode(col, QHeaderView::Fixed);
    }

    if (model->isNumbered) {
      int col = model->numberedCol();
      base::setColumnWidth(col, mWidth(this, model->isNumbered));
      base::header()->setSectionResizeMode(col, QHeaderView::Fixed);
    }
  }
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
  return mWidth(this, 1.6);
}

//------------------------------------------------------------------------------
}
// eof
