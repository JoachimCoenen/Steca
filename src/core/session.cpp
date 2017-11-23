/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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
#include "calc/fit_params.hpp"
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
}

void Session::load(io::Json::rc) may_err {
  clear();
}

io::Json Session::save() const {
  return io::Json(io::Json::OBJ);
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
  mut(fp->lazyIntensCorr).drop();
  mut(fp->corrEnabled) = true;
  mut(corrFile).reset(file.takeOwn());
}

void Session::remCorrFile() {
  if (!corrFile)
    return;

  mut(corrFile).drop();
  mut(fp->corrImage).drop();
  mut(fp->lazyIntensCorr).drop();
  mut(fp->corrEnabled) = false;

  updateImageSize();
}

void Session::tryEnableCorr(bool on) {
  mut(fp->corrEnabled) = on && corrFile;
}

void Session::setBg(Ranges::rc rs) {
  mut(fp->bgRanges) = rs;
}

void Session::addBg(Range::rc r) {
  mut(fp->bgRanges).add(r);
}

void Session::remBg(Range::rc r) {
  mut(fp->bgRanges).rem(r);
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
