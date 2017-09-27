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
#include <algorithm>

namespace core {
//------------------------------------------------------------------------------

str_vec const normStrLst({"none", "monitor", "Δ monitor", "Δ time", "background"});

Session::Session() : files(), corrFile(), fp() {
  clear();
}

void Session::clear() {
  mut(files).reset(new data::Files);
  mut(corrFile).drop();
  mut(fp).reset(new calc::FitParams);
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

void Session::load(io::Json::rc) may_err {
  clear();
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
//    calc::shpp_Reflection reflection(new calc::Reflection);
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

//  auto&& geo = session_->geometry();
//  top.saveObj(config_key::DETECTOR, JsonObj()
//      .savePreal(config_key::DET_DISTANCE, geo.detectorDistance)
//      .savePreal(config_key::DET_PIX_SIZE, geo.pixSize)
//      .saveObj(config_key::BEAM_OFFSET, geo.midPixOffset.saveJson()));

//  auto&& cut = session_->imageCut();
//  top.saveObj(config_key::CUT, JsonObj()
//      .saveUint(config_key::LEFT, cut.left)
//      .saveUint(config_key::TOP, cut.top)
//      .saveUint(config_key::RIGHT, cut.right)
//      .saveUint(config_key::BOTTOM, cut.bottom));

//  auto&& trn = session_->imageTransform();
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
//  for (auto&& reflection : reflections())
//    arrReflections.append(reflection->saveJson());

//  top.saveArr(config_key::REFLECTIONS, arrReflections);

//  return QJsonDocument(top.sup()).toJson();
}

bool Session::addFiles(l_io::path_vec::rc ps) may_err {
  l_io::busy __;
  bool res = false;
  for (auto&& path : ps)
    if (!files->hasPath(path)) {
      auto&& file = l::scope(io::load(path));
      setImageSize(file->imageSize());
      mut(*files).addFile(l::sh(file.takeOwn()));
      res = true;
    }

  return res;
}

bool Session::remFilesAt(uint_vec::rc is) {
  if (is.isEmpty())
    return false;

  uint_vec iis = is;
  std::sort(iis.begin(), iis.end());

  for_i_down_(uint(iis.size()))
    mut(*files).remFileAt(iis.at(i));

  return true;
}

bool Session::activateFileAt(uint i, bool on) {
  if (i >= files->size() || files->at(i)->isActive == on)
    return false;

  mut(files->at(i)->isActive) = on;
  return true;
}

void Session::setCorrFile(l_io::path::rc path) may_err {
  EXPECT_(!path.isEmpty())

  l_io::busy __;

  auto&& file = l::scope(io::load(path));
  auto&& sets = file->sets;

  setImageSize(sets.imageSize());

  mut(fp->corrImage)   = sets.foldImage();
  mut(fp->intensCorrImage).drop();
  mut(fp->corrEnabled) = true;
  mut(corrFile).reset(file.takeOwn());
}

void Session::remCorrFile() {
  if (!corrFile)
    return;

  mut(corrFile).drop();
  mut(fp->corrImage).drop();
  mut(fp->intensCorrImage).drop();
  mut(fp->corrEnabled) = false;

  updateImageSize();
}

void Session::tryEnableCorr(bool on) {
  mut(fp->corrEnabled) = on && corrFile;
}

void Session::setBg(Ranges::rc rs) {
  mut(fp->bg) = rs;
}

void Session::addBg(Range::rc r) {
  mut(fp->bg).add(r);
}

void Session::remBg(Range::rc r) {
  mut(fp->bg).rem(r);
}

void Session::setRefl(Range::rc r) {
  auto&& refl = fp->currRefl;
  if (refl)
    mut(*refl).setRange(r);
}

Session::ref Session::setImageSize(l::sz2 size) may_err {
  auto&& g = fp->geometry;
  if (g.imageSize.isEmpty())
    mut(g.imageSize) = size;  // the first one
  else if (g.imageSize != size)
    l::err("inconsistent image size");
  RTHIS
}

void Session::updateImageSize() {
  if (0 == files->size() && !corrFile)
    mut(fp->geometry.imageSize) = l::sz2(0, 0);
}

//------------------------------------------------------------------------------
}
// eof
