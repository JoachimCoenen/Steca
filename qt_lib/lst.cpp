// (qt_lib)

#include "lst.hpp"
#include <dev_lib/defs.inc>

#include <QHeaderView>
#include <QKeyEvent>

namespace l_qt {
//------------------------------------------------------------------------------

lst_view::lst_view() : model(nullptr), isCheckable(false), hasHeader(false) {
  base::setSelectionBehavior(SelectRows);
  base::setAlternatingRowColors(true);

  connect(this, &Self::clicked, [this](QModelIndex const& index) {
    checkRow(index);
  });
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
    base::header()->setVisible(hasHeader);

    con = connect(model, &QAbstractTableModel::modelReset, [this]() {
      sizeColumns();
    });

    sizeColumns();
    selectRow(rw_n(0));
  }

  RTHIS
}

lst_view::ref lst_view::setCheckable(bool on) SET_(mut(isCheckable) = on)

lst_view::ref lst_view::showHeader(bool on) {
  mut(hasHeader) = on; RTHIS
}

int lst_view::currentRow() const {
  return base::currentIndex().row();
}

lst_view::ref lst_view::selectRow(rw_n rw) {
  EXPECT_(base::model())
  base::setCurrentIndex(base::model()->index(int(rw), 1));
  RTHIS
}

int lst_view::selectedRow() const {
  auto rws = selectedRows();
  return rws.isEmpty() ? -1 : rws.first();
}

lst_view::ref lst_view::selectRows(l::vec<rw_n> rws) {
  EXPECT_(base::model())
  auto m   = base::model();
  int cols = m->columnCount();

  QItemSelection is;
  for (auto rw : rws)
    is.append(QItemSelectionRange(m->index(int(rw), 0),
                                  m->index(int(rw), cols - 1)));
  selectionModel()->select(is, QItemSelectionModel::ClearAndSelect);
  RTHIS
}

l::vec<lst_view::rw_n> lst_view::selectedRows() const {
  l::vec<lst_view::rw_n> rws;
  for (auto& index : selectionModel()->selectedRows())
    rws.add(rw_n(l::to_u(index.row())));
  return rws;
}

lst_view::ref lst_view::sizeColumns() {
  if (!model)
    RTHIS

  base::hideColumn(0);  // should look like a list; 0th column is tree-like

  for_i_(int(model->cols()))
    base::resizeColumnToContents(i + int(model->colOff()));

  if (model->isCheckable) {
    base::setColumnWidth(1, int(1.6 * fontMetrics().width('m')));
    base::header()->setSectionResizeMode(1, QHeaderView::Fixed);
  } else {
    base::header()->setSectionResizeMode(1, QHeaderView::Interactive);
  }

  RTHIS
}

void lst_view::keyPressEvent(QKeyEvent* e) {
  switch (e->nativeVirtualKey()) {
  case Qt::Key_Space:
    checkRow(base::currentIndex());
    return;
  default:
    base::keyPressEvent(e);
  }
}

void lst_view::checkRow(QModelIndex const& index) {
  auto rw = index.row();
  if (rw >= 0 && 1==index.column())
    checkRow(int(rw));
}

void lst_view::checkRow(int row) {
  EXPECT_(model)
  if (0 <= row) {
    auto rw = rw_n(row);
    EXPECT_(rw < model->rows())
    mutp(model)->check(rw);
    QModelIndex index = base::model()->index(int(rw), 1);
    emit dataChanged(index, index);
  }
}

int lst_view::sizeHintForColumn(int) const {
  return int(3 * fontMetrics().width('m'));
}

//------------------------------------------------------------------------------
}
// eof
