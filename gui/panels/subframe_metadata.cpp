// ************************************************************************** //
//
//  Steca: stress and texture calculator
//
//! @file      gui/panels/subframe_metadata.cpp
//! @brief     Implements class SubframeMetadata
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#include "gui/panels/subframe_metadata.h"
#include "gui/base/table_model.h"
#include "gui/thehub.h"
#include "gui/base/tree_views.h" // inheriting from
#include <QHeaderView>


// ************************************************************************** //
//  local class MetadataModel
// ************************************************************************** //

class MetadataModel : public TableModel {
public:
    MetadataModel();

    void reset(const Cluster* cluster);
    void flipCheck(int row);

    int columnCount() const final { return NUM_COLUMNS; }
    int rowCount() const final { return Metadata::numAttributes(false); }
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const { return {}; }
    vec<bool> const& rowsChecked() const { return rowsChecked_; }

    enum { COL_CHECK = 1, COL_TAG, COL_VALUE, NUM_COLUMNS };

private:
    shp_Metadata metadata_;
    vec<bool> rowsChecked_;
};


MetadataModel::MetadataModel() {
    rowsChecked_.fill(false, Metadata::numAttributes(false));
}

void MetadataModel::reset(const Cluster* cluster) {
    metadata_.clear();
    if (cluster)
        metadata_ = cluster->avgeMetadata();
    signalReset();
}

void MetadataModel::flipCheck(int row) {
    rowsChecked_[row] = !rowsChecked_[row];
    signalReset();
}

QVariant MetadataModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || rowCount() <= row)
        return {};
    int col = index.column();
    switch (role) {
    case Qt::CheckStateRole:
        switch (col) {
        case COL_CHECK:
            return rowsChecked_.at(row) ? Qt::Checked : Qt::Unchecked;
        }
        break;
    case Qt::DisplayRole:
        switch (col) {
        case COL_TAG:
            return Metadata::attributeTag(row, false);
        case COL_VALUE:
            return metadata_ ? metadata_->attributeStrValue(row) : "-";
        }
        break;
    }
    return {};
}


// ************************************************************************** //
//  local class MetadataView
// ************************************************************************** //

class MetadataView : public ListView {
public:
    MetadataView();

private:
    int sizeHintForColumn(int) const final;
    MetadataModel* model() const final { return static_cast<MetadataModel*>(ListView::model()); }
};

MetadataView::MetadataView() : ListView() {
    setHeaderHidden(true);
    auto metadataModel = new MetadataModel();
    setModel(metadataModel);
    connect(gHub, &TheHub::sigClusterSelected, model(), &MetadataModel::reset);
    connect(this, &MetadataView::clicked, [this](QModelIndex const& index) {
        model()->flipCheck(index.row());
        emit gHub->sigMetatagsChosen(model()->rowsChecked());
    });
}

int MetadataView::sizeHintForColumn(int col) const {
    switch (col) {
    case MetadataModel::COL_CHECK:
        return 2*mWidth();
    default:
        return 3*mWidth();
    }
}

// ************************************************************************** //
//  class SubframeMetadata
// ************************************************************************** //

SubframeMetadata::SubframeMetadata() : DockWidget("Metadata", "dock-metadata") {
    box_->addWidget((metadataView_ = new MetadataView()));
}