// ************************************************************************** //
//
//  Steca2: stress and texture calculator
//
//! @file      gui/thehub.h
//! @brief     Defines class TheHub
//!
//! @homepage  https://github.com/scgmlz/Steca2
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2017
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#ifndef THEHUB_H
#define THEHUB_H

#include "actions.h"
#include "def/special_pointers.h"
#include "models.h"
#include "session.h"
#include "typ/exception.h"
#include "typ/range.h"
#include "typ/str.h"

class QSpinBox;
class QDoubleSpinBox;

namespace gui {

class TheHub : public TheHubSignallingBase {
private:
    using super = TheHubSignallingBase;
    friend class TheHubSignallingBase;
public:
    TheHub();

    Actions actions;

private:
    void configActions();

    scoped<core::Session*> session_;

    bool isFixedIntenImageScale_;
    bool isFixedIntenDgramScale_;
    bool isCombinedDgram_;

public:
    bool isFixedIntenImageScale() const { return isFixedIntenImageScale_; }
    bool isFixedIntenDgramScale() const { return isFixedIntenDgramScale_; }
    bool isCombinedDgram() const { return isCombinedDgram_; }

    models::FilesModel filesModel;
    models::DatasetsModel datasetsModel;
    models::MetadataModel metadataModel;
    models::ReflectionsModel reflectionsModel;

    // files
    uint numFiles() const { return session_->numFiles(); }
    str fileName(uint index) const { return getFile(index)->fileName(); }
    str filePath(uint index) const { return getFile(index)->fileInfo().absoluteFilePath(); }
    data::shp_File getFile(uint index) const { return session_->file(index); }
    void remFile(uint);

    bool hasCorrFile() const;
    typ::shp_Image corrImage() const { return session_->corrImage(); }

    calc::shp_ImageLens plainImageLens(typ::Image const&) const;
    calc::shp_DatasetLens datasetLens(data::Dataset const&) const;

    typ::Curve avgCurve(data::Datasets const& dss) const { return dss.avgCurve(*session_); }

    calc::ReflectionInfos makeReflectionInfos(
        calc::Reflection const&, uint gmaSlices, typ::Range const&, Progress*);

    void saveSession(QFileInfo const&) const;
    QByteArray saveSession() const;

    void clearSession();
    void loadSession(QFileInfo const&) THROWS;
    void loadSession(QByteArray const&) THROWS;

    void addGivenFile(rcstr filePath) THROWS;
    void addGivenFiles(str_lst const& filePaths) THROWS;

private:
    uint_vec collectFromFiles_;
    pint datasetsGroupedBy_ = pint(1);

public:
    void collectDatasetsFromFiles(uint_vec, pint);
    void collectDatasetsFromFiles(uint_vec);
    void combineDatasetsBy(pint);

    uint_vec const& collectedFromFiles() const { return session_->collectedFromFiles(); }

    pint datasetsGroupedBy() const { return datasetsGroupedBy_; }

    uint numCollectedDatasets() const { return collectedDatasets().count(); }

    data::Datasets const& collectedDatasets() const { return session_->collectedDatasets(); }

    str_lst const& collectedDatasetsTags() const { return session_->collectedDatasetsTags(); }

    typ::size2d imageSize() const { return session_->imageSize(); }

    typ::shp_AngleMap angleMap(data::OneDataset const& dataset) const {
        return session_->angleMap(dataset);
    }

    typ::Range collectedDatasetsRgeGma() const;

    void setCorrFile(rcstr filePath) THROWS;
    void tryEnableCorrection(bool);

    typ::ImageCut const& imageCut() const;
    void setImageCut(bool isTopOrLeft, bool linked, typ::ImageCut const&);

    typ::Geometry const& geometry() const;
    void setGeometry(preal detectorDistance, preal pixSize, typ::IJ const& midPixOffset);

    typ::Range const& gammaRange() const { return session_->gammaRange(); }
    void setGammaRange(typ::Range const&);

    void setBgRanges(typ::Ranges const&);
    void addBgRange(typ::Range const&);
    void remBgRange(typ::Range const&);

    static uint constexpr MAX_POLYNOM_DEGREE = 4;
    void setBgPolyDegree(uint);

    void setIntenScaleAvg(bool, preal);

    void setReflType(fit::ePeakType);

    void addReflection(fit::ePeakType);
    void remReflection(uint);

    eFittingTab fittingTab() const { return fittingTab_; }
    void setFittingTab(eFittingTab);

private:
    eFittingTab fittingTab_ = eFittingTab::NONE;

    data::shp_Dataset selectedDataset_;
    calc::shp_Reflection selectedReflection_;

public:
    data::shp_Dataset selectedDataset() const { return selectedDataset_; }

private:
    void setImageRotate(typ::ImageTransform);
    void setImageMirror(bool);

public:
    void setNorm(eNorm);

    typ::Ranges const& bgRanges() const { return session_->bgRanges(); }
    uint bgPolyDegree() const { return session_->bgPolyDegree(); }

    bool intenScaledAvg() const { return session_->intenScaledAvg(); }
    preal intenScale() const { return session_->intenScale(); }

    calc::Reflections const& reflections() const { return session_->reflections(); }
};

} // namespace gui

#endif
