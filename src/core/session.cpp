/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
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

#include "session.hpp"
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>
#include "fit/fit_methods.hpp"
#include "io/io.hpp"

namespace core {
//------------------------------------------------------------------------------

str_vec const normStrLst({"none", "monitor", "Δ monitor", "Δ time", "background"});

Session::Session()
: angleMapKey0()
, imageTransform(), imageCut(), imageSize()
, avgScaleIntens(), intenScale(1)
, corrEnabled(false), corrFile(), corrImage()
, collectedDatasets(), collectedDatasetsTags(), collectedFromFiles(), groupedBy(1)
, bgPolyDegree(0), bgRanges()
, angleMapCache(l::pint(12)), intensCorrImage(), corrHasNaNs()
{}

Session::ref Session::clear() {
  RTHIS
  //TODO
//  while (0 < numFiles())
//    remFile(0);

//  remCorrFile();
//  corrEnabled_ = corrHasNaNs_ = false;

//  bgPolyDegree_ = 0;
//  bgRanges_.clear();

//  reflections_.clear();

//  norm_ = eNorm::NONE;

//  angleMapCache_.clear();

//  intenScaledAvg_ = true;
//  intenScale_ = preal(1);
}

Session::ref Session::load(io::Json::rc) may_err {
  RTHIS
//  QJsonParseError parseError;
//  QJsonDocument   doc(QJsonDocument::fromJson(json, &parseError));
//  RUNTIME_CHECK(QJsonParseError::NoError == parseError.error,
//                "Error parsing session file");

//  TakesLongTime __;

//  clearSession();

//  typ::JsonObj top(doc.object());

//  auto files = top.loadArr(config_key::FILES);
//  for (auto file : files) {
//    str  filePath = file.toString();
//    QDir dir(filePath);
//    RUNTIME_CHECK(dir.makeAbsolute(),
//                  str("Invalid file path: %1").arg(filePath));
//    addFile(dir.absolutePath());
//  }

//  auto sels = top.loadArr(config_key::SELECTED_FILES, true);
//  uint_vec selIndexes;
//  for (auto sel : sels) {
//    int i = sel.toInt(), index = qBound(0, i, to_i(files.count()));
//    RUNTIME_CHECK(i == index, str("Invalid selection index: %1").arg(i));
//    selIndexes.append(to_u(index));
//  }

//  std::sort(selIndexes.begin(), selIndexes.end());
//  int lastIndex = -1;
//  for (uint index : selIndexes) {
//    RUNTIME_CHECK(lastIndex < to_i(index), str("Duplicate selection index"));
//    lastIndex = to_i(index);
//  }

//  collectDatasetsFromFiles(selIndexes,top.loadPint(config_key::COMBINE,1));

//  setCorrFile(top.loadString(config_key::CORR_FILE, EMPTY_STR));

//  auto det = top.loadObj(config_key::DETECTOR);
//  setGeometry(det.loadPreal(config_key::DET_DISTANCE), det.loadPreal(config_key::DET_PIX_SIZE),
//              det.loadIJ(config_key::BEAM_OFFSET));

//  auto cut = top.loadObj(config_key::CUT);
//  uint x1 = cut.loadUint(config_key::LEFT),  y1 = cut.loadUint(config_key::TOP),
//       x2 = cut.loadUint(config_key::RIGHT), y2 = cut.loadUint(config_key::BOTTOM);
//  setImageCut(true, false, typ::ImageCut(x1, y1, x2, y2));

//  setImageRotate(typ::ImageTransform(top.loadUint(config_key::TRANSFORM)));

//  typ::Ranges bgRanges;
//  bgRanges.loadJson(top.loadArr(config_key::BG_RANGES));
//  setBgRanges(bgRanges);

//  setBgPolyDegree(top.loadUint(config_key::BG_DEGREE));

//  setIntenScaleAvg(top.loadBool(config_key::INTEN_SCALED_AVG, true),
//                   top.loadPreal(config_key::INTEN_SCALE, preal(1)));

//  auto reflectionsObj = top.loadArr(config_key::REFLECTIONS);
//  for_i (reflectionsObj.count()) {
//    calc::shp_Reflection reflection(new calc::Reflection);
//    reflection->loadJson(reflectionsObj.objAt(i));
//    session_->addReflection(reflection);
//  }

//  emit sigReflectionsChanged();
}

io::Json Session::save() const {
  return io::Json(io::Json::OBJ);
//  using typ::JsonObj;
//  using typ::JsonArr;

//  JsonObj top;

//  auto& geo = session_->geometry();
//  top.saveObj(config_key::DETECTOR, JsonObj()
//      .savePreal(config_key::DET_DISTANCE, geo.detectorDistance)
//      .savePreal(config_key::DET_PIX_SIZE, geo.pixSize)
//      .saveObj(config_key::BEAM_OFFSET, geo.midPixOffset.saveJson()));

//  auto& cut = session_->imageCut();
//  top.saveObj(config_key::CUT, JsonObj()
//      .saveUint(config_key::LEFT, cut.left)
//      .saveUint(config_key::TOP, cut.top)
//      .saveUint(config_key::RIGHT, cut.right)
//      .saveUint(config_key::BOTTOM, cut.bottom));

//  auto& trn = session_->imageTransform();
//  top.saveUint(config_key::TRANSFORM, trn.val);

//  JsonArr arrFiles;
//  // save file path relative to location of session
//  for_i (numFiles()) {
//    str absPath = getFile(i)->fileInfo().absoluteFilePath();
//    str relPath = QDir::current().relativeFilePath(absPath);
//    arrFiles.append(relPath);
//  }

//  top.saveArr(config_key::FILES, arrFiles);

//  JsonArr arrSelectedFiles;
//  for (uint i : collectedFromFiles())
//    arrSelectedFiles.append(to_i(i));

//  top.saveArr(config_key::SELECTED_FILES, arrSelectedFiles);
//  top.saveUint(config_key::COMBINE, datasetsGroupedBy_);

//  if (hasCorrFile()) {
//    str absPath = session_->corrFile()->fileInfo().absoluteFilePath();
//    str relPath = QDir::current().relativeFilePath(absPath);
//    top.saveString(config_key::CORR_FILE, relPath);
//  }

//  top.saveUint(config_key::BG_DEGREE, bgPolyDegree());
//  top.saveArr(config_key::BG_RANGES, bgRanges().saveJson());
//  top.saveBool(config_key::INTEN_SCALED_AVG, intenScaledAvg());
//  top.savePreal(config_key::INTEN_SCALE, intenScale());

//  JsonArr arrReflections;
//  for (auto& reflection : reflections())
//    arrReflections.append(reflection->saveJson());

//  top.saveArr(config_key::REFLECTIONS, arrReflections);

//  return QJsonDocument(top.sup()).toJson();
}

AngleMap::sh Session::angleMap(data::Set::rc set) const {
  AngleMap::Key key(angleMapKey0, set.tth());
  auto map = angleMapCache.get(key);
  if (!map)
    map = angleMapCache.add(key, AngleMap::sh(new AngleMap(key)));
  return map;
}

Image::sh Session::intensCorr() const {
  if (!corrEnabled)
    return Image::sh();

  if (!intensCorrImage)
    calcIntensCorr();

  return intensCorrImage;
}

bool Session::hasFile(l_io::path::rc path) const {
  for (auto& file : files)
    if (file->path == path)
      return true;

  return false;
}

Session::ref Session::addFile(l_io::path::rc path) may_err {
  if (!path.isEmpty()) {
    auto file = io::load(files, path);
    setImageSize(file->sets.imageSize());
    mut(files).addFile(file);
  }
  RTHIS
}

Session::ref Session::remFile(uint i) {
  mut(files).rem(i);
  updateImageSize();
  if (files.isEmpty())
    mut(files.dict).reset(new data::Meta::Dict());
  RTHIS
}

Session::ref Session::activateFileAt(uint i, bool on) {
  mut(files.at(i)->isActive) = on;
  RTHIS
}

bool Session::isActiveFileAt(uint i) const {
  return files.at(i)->isActive;
}

Session::ref Session::activateDatasetAt(uint i, bool on) {
  mut(collectedDatasets.at(i)->isActive) = on;
  RTHIS
}

bool Session::isActiveDatasetAt(uint i) const {
  return collectedDatasets.at(i)->isActive;
}

Session::ref Session::setCorrFile(l_io::path::rc path) may_err {
  if (path.isEmpty()) {
    remCorrFile();
  } else {
    l_io::busy __;

    auto file = io::load(mut(files), path);
    auto& sets = file->sets;

    setImageSize(sets.imageSize());
    mut(corrImage) = sets.foldImage();
    intensCorrImage.drop();

    // all ok
    mut(corrFile)    = file;
    mut(corrEnabled) = true;
  }
  RTHIS
}

Session::ref Session::remCorrFile() {
  mut(corrFile).drop();
  mut(corrImage).drop();
  mut(intensCorrImage).drop();
  mut(corrEnabled) = false;
  updateImageSize();
  RTHIS
}

Session::ref Session::tryEnableCorr(bool on) {
  mut(corrEnabled) = on && corrFile;
  RTHIS
}

Session::ref Session::collectDatasetsFromFiles(uint_vec::rc is, l::pint by) {
  mut(collectedFromFiles) = is;
  mut(collectedDatasets).clear();
  mut(collectedDatasetsTags).clear();
  mut(groupedBy) = by;

  auto cs = l::scope(new data::CombinedSet);
  uint i = 0;

  auto appendCs = [this, &cs, &i]() {
    auto sz = cs->size();
    if (!sz)
      return;

    str tag = str::num(i + 1); i += sz;
    if (groupedBy > 1)
      tag += '-' + str::num(i);

    mut(collectedDatasets).add(cs.takeOwn());
    mut(collectedDatasetsTags).add(tag);
    cs.reset(new data::CombinedSet);
  };

  auto gb = groupedBy;
  for (uint i : collectedFromFiles)
    for (auto& set : files.at(i)->sets) { // Set::sh
      cs->add(set);
      if (0 == --gb) {
        appendCs();
        gb = groupedBy;
      }
    }

  appendCs();  // the potentially remaining ones

  RTHIS
}

Session::ref Session::setImageSize(l::sz2 size) may_err {
  if (imageSize.isEmpty())
    mut(imageSize) = size;  // the first one
  else if (imageSize != size)
    l::err("inconsistent image size");
  RTHIS
}


calc::ImageLens::sh Session::imageLens(
  Image::rc image, data::CombinedSets::rc datasets, bool trans, bool cut) const
{
  return l::share(new calc::ImageLens(*this, image, datasets, trans, cut));
}

Curve Session::makeCurve(calc::DatasetLens::rc lens, gma_rge::rc rgeGma) const {
  Curve curve = lens.makeCurve(rgeGma);
  curve.subtract(fit::Polynom::fromFit(bgPolyDegree, curve, bgRanges));

  return curve;
}

calc::DatasetLens::sh Session::datasetLens(
    data::CombinedSet::rc dataset, data::CombinedSets::rc datasets,
    eNorm norm, bool trans, bool cut) const {
  return l::share(new calc::DatasetLens(*this, dataset, datasets, norm,
                         trans, cut, imageTransform, imageCut));
}

real Session::calcAvgBackground(data::CombinedSet::rc dataset) const {
  EXPECT_(dataset.parent)
  auto lens = datasetLens(dataset, *dataset.parent, eNorm::NONE, true, true);

  Curve gmaCurve = lens->makeCurve(true); // REVIEW averaged?
  auto bgPolynom = fit::Polynom::fromFit(bgPolyDegree, gmaCurve, bgRanges);
  return bgPolynom.avgY(lens->rgeTth());
}

real Session::calcAvgBackground(data::CombinedSets::rc datasets) const {
  if (datasets.isEmpty())
    return 0;

  l_io::busy __;

  real bg = 0;

  for (auto& dataset : datasets)
    bg += calcAvgBackground(*dataset);

  return bg / datasets.size();
}

void Session::updateImageSize() {
  if (0 == files.size() && !corrFile)
    mut(imageSize) = l::sz2(0, 0);
}

void Session::calcIntensCorr() const {
  corrHasNaNs = false;

  EXPECT_(corrImage)
  l::sz2 size = corrImage->size() - imageCut.marginSize();
  ENSURE_(!size.isEmpty())

  uint w = size.i, h = size.j,
       di = imageCut.left, dj = imageCut.top;

  real sum = 0;
  for_ij_(w, h)
    sum += corrImage->inten(i + di, j + dj);

  real avg = sum / (w * h);

  intensCorrImage = l::share(new Image(corrImage->size(), inten_t(1)));

  for_ij_(w, h) {
    auto inten = corrImage->inten(i + di, j + dj);
    real factor;

    if (inten > 0) {
      factor = avg / inten;
    } else {
      factor = l::flt_nan;
      corrHasNaNs = true;
    }

    mutp(intensCorrImage)->setInten(i + di, j + dj, inten_t(factor));
  }
}

//------------------------------------------------------------------------------
}
// eof
