// (qt_lib)

#include "lst.h"
#include <dev_lib/inc/defs_cpp.h>

#include <QHeaderView>
#include <QKeyEvent>

namespace l_qt {
//------------------------------------------------------------------------------

lst_view::lst_view() : isCheckable(false), hasHeader(false), model(nullptr) {
  base::setSelectionBehavior(SelectRows);
  base::setAlternatingRowColors(true);

  connect(this, &Self::clicked, [this](QModelIndex const& index) {
    checkRow(index);
  });
}

lst_view::ref lst_view::set(lst_model* model_) {
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
    selectRow(lst_model::rw_n(0));
  }

  RT
}

lst_view::ref lst_view::setCheckable(bool on) {
  mut(isCheckable) = on; RT
}

lst_view::ref lst_view::showHeader(bool on) {
  mut(hasHeader) = on; RT
}

lst_view::ref lst_view::selectRow(lst_model::rw_n rw) {
  EXPECT_(base::model())
  base::setCurrentIndex(base::model()->index(int(rw), 1));
  RT
}

int lst_view::selectedRow() const {
  return base::currentIndex().row();
}

lst_view::ref lst_view::sizeColumns() {
  if (!model)
    RT

  base::hideColumn(0);  // should look like a list; 0th column is tree-like

  for_i_(int(model->cols()))
    base::resizeColumnToContents(i + int(model->colOff()));

  if (model->isCheckable) {
    base::setColumnWidth(1, int(1.6 * fontMetrics().width('m')));
    base::header()->setSectionResizeMode(1, QHeaderView::Fixed);
  } else {
    base::header()->setSectionResizeMode(1, QHeaderView::Interactive);
  }

  RT
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
    auto rw = lst_model::rw_n(row);
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
