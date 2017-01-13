/*******************************************************************************
 * REVIEW: STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#ifndef THEHUB_H
#define THEHUB_H

#include "actions.h"
#include "session.h"
#include "models.h"
#include "def/defs.h"

#include <QAction>
#include <QSettings>

class QSpinBox;
class QDoubleSpinBox;

namespace gui {
//------------------------------------------------------------------------------

class Settings : public QSettings {
  CLASS(Settings) SUPER(QSettings)
public:
  Settings(qstrc group = EMPTY_STR);
 ~Settings();

  QVariant readVariant(qstrc key, QVariant const& def);
  void     saveVariant(qstrc key, QVariant const& val);

  void read(qstrc key, QAction*, bool def = false);
  void save(qstrc key, QAction*);

  void read(qstrc key, QSpinBox*, int def = 0);
  void save(qstrc key, QSpinBox*);

  void read(qstrc key, QDoubleSpinBox*, real def = 0);
  void save(qstrc key, QDoubleSpinBox*);

  bool readBool(qstrc key, bool def = false);
  void saveBool(qstrc key, bool);

  int  readInt(qstrc key, int def = 0);
  void saveInt(qstrc key, int);

  real readReal(qstrc key, real def = 0);
  void  saveReal(qstrc key, real);

  str  readStr(qstrc key, qstrc def = EMPTY_STR);
  void saveStr(qstrc key, qstrc);
};

//------------------------------------------------------------------------------

class ReadFile: public QFile {
  CLASS(ReadFile) SUPER(QFile)
public:
  ReadFile(qstrc path) may_exc;
};

class WriteFile: public QFile {
  CLASS(WriteFile) SUPER(QFile)
public:
  WriteFile(qstrc path) may_exc;
};

//------------------------------------------------------------------------------

class TheHub : public TheHubSignallingBase {
  CLASS(TheHub) SUPER(TheHubSignallingBase)
  friend class TheHubSignallingBase;
public:
  TheHub();

  Actions actions;

private:
  void configActions();

private:
  scoped<core::Session*> session_;

  bool isFixedIntenImageScale_;
  bool isFixedIntenDgramScale_;
  bool isCombinedDgram_;

public:
  bool isFixedIntenImageScale() const { return isFixedIntenImageScale_; }
  bool isFixedIntenDgramScale() const { return isFixedIntenDgramScale_; }
  bool isCombinedDgram()        const { return isCombinedDgram_;        }

  models::FilesModel       filesModel;
  models::DatasetsModel    datasetsModel;
  models::MetadataModel    metadataModel;
  models::ReflectionsModel reflectionsModel;

public: // files
  uint numFiles() const;
  str  fileName(uint index) const;
  str  filePath(uint index) const;
  data::shp_File getFile(uint) const;
  void           remFile(uint);

  bool           hasCorrFile() const;
  typ::Image::rc corrImage() const;

public:
  calc::shp_ImageLens   plainImageLens(typ::Image::rc) const;
  calc::shp_DatasetLens datasetLens(data::Dataset::rc) const;

  typ::Curve avgCurve(data::Datasets::rc, bool averaged) const;

public:
  calc::ReflectionInfos makeReflectionInfos(calc::Reflection::rc,
      uint gmaSlices, gma_rge::rc, Progress*);

public:
  void       saveSession(QFileInfo const&) const;
  QByteArray saveSession() const;

  void clearSession();
  void loadSession(QFileInfo const&)  may_exc;
  void loadSession(QByteArray const&) may_exc;

public:
  void addFile(qstrc filePath) may_exc;
  void addFiles(str_lst::rc filePaths) may_exc;

private:
  uint_vec collectFromFiles_;
  pint     datasetsGroupedBy_ = pint(1);

public:
  void collectDatasetsFromFiles(uint_vec, pint);
  void collectDatasetsFromFiles(uint_vec);
  void combineDatasetsBy(pint);

  uint_vec::rc collectedFromFiles() const {
    return session_->collectedFromFiles();
  }

  pint datasetsGroupedBy() const {
    return datasetsGroupedBy_;
  }

  uint numCollectedDatasets() const {
    return collectedDatasets().count();
  }

  data::Datasets::rc collectedDatasets() const {
    return session_->collectedDatasets();
  }

  str_lst::rc collectedDatasetsTags() const {
    return session_->collectedDatasetsTags();
  }

  typ::size2d imageSize() const {
    return session_->imageSize();
  }

  typ::shp_AngleMap angleMap(data::OneDataset::rc dataset) const {
    return session_->angleMap(dataset);
  }

  gma_rge collectedDatasetsRgeGma() const;

  void setCorrFile(qstrc filePath) may_exc;
  void tryEnableCorrection(bool);

  typ::ImageCut::rc imageCut() const;
  void setImageCut(bool topLeft, bool linked, typ::ImageCut::rc);

  typ::Geometry::rc geometry() const;
  void setGeometry(preal detectorDistance, preal pixSize, typ::IJ::rc midPixOffset);

  typ::Range::rc gammaRange() const;
  void setGammaRange(typ::Range::rc);

public:
  void setBgRanges(typ::Ranges::rc);
  void addBgRange(typ::Range::rc);
  void remBgRange(typ::Range::rc);

  static uint constexpr MAX_POLYNOM_DEGREE = 4;
  void setBgPolyDegree(uint);

  void setReflType(fit::ePeakType);

  void addReflection(fit::ePeakType);
  void remReflection(uint);

  eFittingTab fittingTab() const { return fittingTab_; }
  void setFittingTab(eFittingTab);

private:
  eFittingTab fittingTab_ = eFittingTab::NONE;

private:
  data::shp_Dataset    selectedDataset_;
  calc::shp_Reflection selectedReflection_;

public:
  data::shp_Dataset selectedDataset() const { return selectedDataset_; }

private:
  void setImageRotate(typ::ImageTransform);
  void setImageMirror(bool);

public:
  void setNorm(eNorm);

public:
  typ::Ranges::rc bgRanges()          const { return session_->bgRanges();     }
  uint            bgPolyDegree()      const { return session_->bgPolyDegree(); }

  calc::Reflections::rc reflections() const { return session_->reflections();  }
};

//------------------------------------------------------------------------------
}
#endif
