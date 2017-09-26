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

#include "thehub.hpp"
#include "win.hpp"

#include <core/io/io.hpp>
#include <core/io/json.hpp>
#include <lib/qt/inc/defs.inc>
#include <lib/dev/io/fio.hpp>
#include <lib/dev/io/log.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/hash.hpp>
#include <lib/qt/dlg_file.hpp>
#include <lib/qt/dlg_msg.hpp>

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

namespace gui {
//------------------------------------------------------------------------------

using idx_rc = QModelIndex const&;

dcl_sub_(FileProxyModel, QSortFilterProxyModel)
  int columnCount(idx_rc) const;
  QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;
  QVariant data(idx_rc, int = Qt::DisplayRole) const;

private:
  mutable l::hash<str,str> memInfo;
dcl_end

int FileProxyModel::columnCount(idx_rc) const {
  return 2;
}

QVariant FileProxyModel::headerData(int section, Qt::Orientation o, int role) const {
  if (1 == section && Qt::Horizontal == o && role == Qt::DisplayRole)
    return "Comment";
  return base::headerData(section, o, role);
}

QVariant FileProxyModel::data(idx_rc idx, int role) const {
  if (idx.isValid() && 1 == idx.column()) {
    switch (role) {
    case Qt::TextAlignmentRole:
      return Qt::AlignLeft;
    case Qt::DisplayRole: {
      str comment;
      QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(sourceModel());
      auto ix0 = fileModel->index(mapToSource(idx).row(), 0, mapToSource(idx.parent()));
      QFileInfo info(fileModel->rootDirectory().filePath(fileModel->fileName(ix0)));
      if (info.isFile()) {
        l_io::path path = l_qt::fromQt(info.absoluteFilePath());

        if (memInfo.contains(path))
          comment = memInfo.at(path);
        else if (core::io::couldBeCaress(path))
          comment = "[car] " + core::io::loadCaressComment(path);
        else if (core::io::couldBeMar(path))
          comment = "[mar] ";
        else if (core::io::couldBeTiffDat(path))
          comment = "[tif] ";

        memInfo.add(path, comment);
      }
      return l_qt::toQt(comment);
    }
    }
  }

  return base::data(idx, role);
}

//------------------------------------------------------------------------------

Hub::Hub(Win& win_) : win(win_), acts(*this, win_)
, dgramOptions(), imageOptions()
, modelFiles(new ModelFiles(*this))
, modelDatasets(new ModelDatasets(*this))
, modelMetadata(new ModelMetadata(*this)) {}

Hub::~Hub() {
  delete modelFiles;
}

void Hub::makeDgram(core::Curve& dgram, core::Curve& bgFitted, core::Curve& bg, core::curve_vec& refls,
                    core::data::CombinedSets::rc sets, core::data::CombinedSet const* set,
                    core::calc::FitParams::rc fp, bool combined) const {

  EXPECT_(dgram.isEmpty() && bgFitted.isEmpty() && bg.isEmpty() && refls.isEmpty())

  if (combined)
    dgram = fp.datasetLens(sets, sets.combineAll()(), true, true)
            -> makeCurve();
  else if (set)
    dgram = fp.datasetLens(sets, *set, true, true)
            -> makeCurve(dgramOptions.gammaRange);

  auto&& bgPolynom = core::fit::Polynom::fromFit(fp.bgPolyDegree, dgram, fp.bgRanges);
  for_i_(dgram.size()) {
    auto x = dgram.xs.at(i), y = bgPolynom.y(x);
    bg.add(x, y);
    bgFitted.add(x, dgram.ys.at(i) - y);
  }

  for (auto&& rsh : fp.refls) {
    auto&& fun = *rsh().peakFun;
    fun.fit(bgFitted);

    auto&& rge = fun.range;

    core::Curve c;
    for (auto&& x : bgFitted.xs)
      if (rge.contains(x))
        c.add(x, fun.y(x));

    refls.add(c);
  }
}

void Hub::init() {
  sessionClear();
}

void Hub::sessionClear() {
  base::clear();
  emitFiles(l::sh(base::files->clone()));
}

void Hub::sessionLoad(l_io::path path) may_err {
  l_io::ftin file(path);
  path.cwd();

  using core::io::Json;
  auto json = Json::asSelf(Json::loadFrom(file.asStream()));
  base::load(json);
  emitFiles(l::sh(base::files->clone()));
}

void Hub::sessionSave(l_io::path path) const may_err {
  l_io::ftout file(path);
  path.cwd();

  base::save().saveTo(file.asStream());
}

void Hub::addFiles(l_io::path_vec::rc ps) emits {
  if (base::addFiles(ps))
    emitFiles(l::sh(base::files->clone()));
}

void Hub::addFiles() emits {
  auto names = l_qt::dlgOpenFiles(&mut(win), "Add files", l_io::path::cwd(),
                "Data files (*.dat *.mar*);;All files (*.*)",
                new FileProxyModel);

  if (!names.isEmpty())
    l_io::path(names.at(0)).dir().cd();

  return addFiles(names);
}

void Hub::remFilesAt(uint_vec::rc is) {
  if (base::remFilesAt(is))
    emitFiles(l::sh(base::files->clone()));
}

void Hub::activateFileAt(uint i, bool on) {
  if (base::activateFileAt(i, on))
    emitFiles(l::sh(base::files->clone()));
}

void Hub::corrEnable(bool on) {
  if (on && !corrFile) {
    str name = l_qt::dlgOpenFile(&mut(win), "Select correction file", l_io::path::cwd(),
                 "Data files (*.dat *.mar*);;All files (*.*)");
    if (!name.isEmpty()) {
      l_io::path(name).dir().cd();
      base::setCorrFile(name);
    }
  }

  base::tryEnableCorr(on);
  emitCorr(base::corrFile);
}

void Hub::corrRem() {
  base::remCorrFile();
  emitCorr(base::corrFile);
}

//bool Hub::corrEnabled() const
//  RET_(base::corrEnabled)

str Hub::corrName() const // TODO remove, broadcast corrFile
  RET_(base::corrFile ? base::corrFile->src->path.filename() : str::null)

void Hub::setBg(Ranges::rc rs) emits {
  base::setBg(rs);
// TODO emitFit(base::fit);
}

void Hub::addBg(Range::rc r) emits {
  base::addBg(r);
// TODO emitFit(base::fit);
}

void Hub::remBg(Range::rc r) emits {
  base::remBg(r);
// TODO emitFit(base::fit);
}

void Hub::setRefl(Range::rc r) emits {
  base::setRefl(r);
// TODO emitFit(base::fit);
}

void Hub::setNorm(core::eNorm norm) {
// ... base::remBg(r);
// TODO emitFit(base::fit);
}

void Hub::sendSetsInfo(core::data::CombinedSets::shr sets, core::data::CombinedSet::shp set) {
  emitSetsInfo(SetsInfo(sets, set, l::sh(base::fp->clone())));
}

void Hub::sendMetaChecked(core::data::KeyBag::shr bag) {
  emitMetaChecked(bag);
}

//------------------------------------------------------------------------------
}
// eof
