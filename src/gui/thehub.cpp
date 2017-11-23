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

#include "thehub.hpp"
#include "win.hpp"

#include <core/io/io.hpp>
#include <core/io/json.hpp>
#include <lib/dev/io/fio.hpp>
#include <lib/dev/io/log.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/hash.hpp>
#include <lib/qt/dlg/dlg_file.hpp>
#include <lib/qt/dlg/dlg_msg.hpp>
#include <lib/qt/defs.inc>

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

namespace gui {
//------------------------------------------------------------------------------

using rw_n = l_qt::lst_model::rw_n;
using cl_n = l_qt::lst_model::cl_n;
using namespace core::data;

//------------------------------------------------------------------------------

Hub::HubModel::HubModel(Hub& hub) : RefHub(hub) {}

//------------------------------------------------------------------------------

Hub::ModelFiles::ModelFiles(Hub& hub) : base(hub) {
  setCheckable(true);
  setNumbered(2);
}

cl_n Hub::ModelFiles::cols() const {
  return cl_n(1);
}

rw_n Hub::ModelFiles::rows() const {
  return rw_n(hub.currentFiles().size());
}

str Hub::ModelFiles::head(cl_n cl) const {
  if (0 == cl)
    return "File";
  return str::null;
}

l_qt::var Hub::ModelFiles::cell(rw_n rw, cl_n cl) const {
  if (0 == cl)
    return at(rw)->src->path.filename();
  return str::null;
}

Hub::ModelFiles::ref Hub::ModelFiles::check(rw_n rw, bool on, bool) {
  hub.activateFileAt(rw, on);
  RTHIS
}

bool Hub::ModelFiles::isChecked(rw_n rw) const {
  return at(rw)->isActive;
}

File::shp Hub::ModelFiles::at(rw_n rw) const {
  return hub.currentFiles().at(rw);
}

//------------------------------------------------------------------------------

Hub::ModelDatasets::ModelDatasets(Hub& hub) : base(hub), groupedBy(1) {
  setCheckable(true);
  setNumbered(4);
}

cl_n Hub::ModelDatasets::cols() const {
  // allow 1 empty column even if there are no metacols (looks better)
  return cl_n(numLeadCols() + l::max(1u, uint(hub.currentMetaKeys.size())));
}

rw_n Hub::ModelDatasets::rows() const {
  return rw_n(hub.currentSets().size());
}

str Hub::ModelDatasets::head(cl_n cl) const {
  if (cl < numLeadCols())
    switch (cl) {
    case clFNO: return "F#";
    case clTAG: return "#-#";
    default:    NEVER_RETURN("");
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < hub.currentMetaKeys.size())
    return hub.currentMetaKeys.at(i);
  return str::null;
}

l_qt::var Hub::ModelDatasets::cell(rw_n rw, cl_n cl) const {
  auto&& set = hub.currentSets().at(rw);

  if (cl < numLeadCols())
    switch (cl) {
    case clFNO: return set().fileNo;
    case clTAG: return set().tag;
    default:    NEVER_RETURN(0)
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < hub.currentMetaKeys.size()) {
    auto&& meta = set().meta();
    int idx = meta->dict->safeIndex(hub.currentMetaKeys.at(i));
    if (0 <= idx)
      return meta->vals.valAt(uint(idx));
  }
  return l_qt::var();
}

bool Hub::ModelDatasets::rightAlign(cl_n cl) const {
  return 0 == cl;
}

Hub::ModelDatasets::ref Hub::ModelDatasets::check(rw_n rw, bool on, bool) {
  auto&& isActive = hub.currentSets().at(rw)().isActive;
  if (isActive != on) {
    mut(isActive) = on;
    signalRowChanged(rw);
  }
  RTHIS
}

bool Hub::ModelDatasets::isChecked(rw_n rw) const {
  return hub.currentSets().at(rw)().isActive;
}

void Hub::ModelDatasets::groupBy(l::pint by) {
  mut(groupedBy) = by;
  combineSets();
}

void Hub::ModelDatasets::setSetAt(int row) const {
  EXPECT_(row < 0 || uint(row) < hub.currentSets().size())
  hub.setSet(0 <= row ? hub.currentSets().at(uint(row)) : CombinedSet::shp());
}

uint Hub::ModelDatasets::numLeadCols() const {
  return 1 < groupedBy ? 2 : 1;
}

void Hub::ModelDatasets::combineSets() {
  hub.currentSets = hub.currentFiles().collectDatasets(groupedBy);
  signalReset();
}

//------------------------------------------------------------------------------

Hub::ModelMetadata::ModelMetadata(Hub& hub) : base(hub), checked(new KeyBag) {
  setCheckable(true);
  hub.onSigSetsInfo([this](Hub::SetsInfo info) {
    if (set != info.set) {
      set = info.set;
      signalReset();
    }
  });
}

cl_n Hub::ModelMetadata::cols() const {
  return cl_n(2);
}

rw_n Hub::ModelMetadata::rows() const {
  return rw_n(hub.currentDict().size());
}

str Hub::ModelMetadata::head(cl_n cl) const {
  switch (cl) {
  case clTAG: return "tag";
  case clVAL: return "val";
  default:    NEVER_RETURN("");
  }
}

l_qt::var Hub::ModelMetadata::cell(rw_n rw, cl_n cl) const {
  EXPECT_(rw < hub.currentDict().size());
  auto&& key = hub.currentDict().key(rw);

  switch (cl) {
  case clTAG:
    return key;
  case clVAL: {
    if (!set)
      return l_qt::var();
    auto&& meta = set->meta();
    auto&& idx  = meta->dict->safeIndex(key);
    return idx < 0 ? l_qt::var() : meta->vals.valAt(uint(idx));
  }
  default:
    NEVER_RETURN(0)
  }
}

Hub::ModelMetadata::ref Hub::ModelMetadata::check(rw_n rw, bool on, bool done) {
  if (mut(*checked).set(hub.currentDict().key(rw), on)) {
    signalRowChanged(rw);
    if (done)
      hub.setMetaChecked(checked);
  }
  RTHIS
}

bool Hub::ModelMetadata::isChecked(rw_n rw) const {
  return checked().contains(hub.currentDict().key(rw));
}

//------------------------------------------------------------------------------

using rcIndex = QModelIndex const&;

dcl_sub_(FileProxyModel, QSortFilterProxyModel)
  int columnCount(rcIndex) const;
  QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;
  QVariant data(rcIndex, int = Qt::DisplayRole) const;

private:
  mutable l::hash<str,str> memInfo;
dcl_end

int FileProxyModel::columnCount(rcIndex) const {
  return 2;
}

QVariant FileProxyModel::headerData(int section, Qt::Orientation o, int role) const {
  if (1 == section && Qt::Horizontal == o && role == Qt::DisplayRole)
    return "Comment";
  return base::headerData(section, o, role);
}

QVariant FileProxyModel::data(rcIndex idx, int role) const {
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
, currentSets()
, modelFiles(new ModelFiles(*this))
, modelDatasets(new ModelDatasets(*this))
, modelMetadata(new ModelMetadata(*this))
{}

void Hub::setSet(core::data::CombinedSet::shp set) {
  if (currentSet != set) {
    currentSet = set;
    sendSetsInfo();
  }
}

void Hub::makeDgram(core::Curve& dgram, core::Curve& bgFitted, core::Curve& bg, core::curve_vec& refls,
                    CombinedSets::rc sets, CombinedSet const* set,
                    core::calc::FitParams::rc fp, bool combined) const {

  EXPECT_(dgram.isEmpty() && bgFitted.isEmpty() && bg.isEmpty() && refls.isEmpty())

  if (combined)
    dgram = fp.datasetLens(sets, sets.combineAll()(), true, true)().makeCurve();
  else if (set)
    dgram = fp.datasetLens(sets, *set, true, true)().makeCurve(dgramOptions.gammaRange);

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
  corrRem(); // to reset controls
  filesModified();
}

void Hub::sessionLoad(l_io::path path) may_err {
  l_io::ftin file(path);
  path.cwd();

  using core::io::Json;
  auto json = Json::asSelf(Json::loadFrom(file.asStream()));
  base::load(json);
  filesModified();
}

void Hub::sessionSave(l_io::path path) const may_err {
  l_io::ftout file(path);
  path.cwd();

  base::save().saveTo(file.asStream());
}

void Hub::addFiles(l_io::path_vec::rc ps) emits {
  if (base::addFiles(ps))
    filesModified();
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
    filesModified();
}

void Hub::activateFileAt(uint i, bool on) {
  if (base::activateFileAt(i, on))
    filesModified();
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

  emitCorrFileName(corrFile ? corrFile->src->path.filename() : str::null);
  base::tryEnableCorr(on);
  sendSetsInfo();
}

void Hub::corrRem() {
  base::remCorrFile();
  emitCorrFileName(str::null);
  sendSetsInfo();
}

void Hub::setBg(Ranges::rc rs) emits {
  base::setBg(rs);
  // emit ...
}

void Hub::addBg(Range::rc r) emits {
  base::addBg(r);
  // emit ...
}

void Hub::remBg(Range::rc r) emits {
  base::remBg(r);
  // emit ...
}

void Hub::setRefl(Range::rc r) emits {
  base::setRefl(r);
  // emit ...
}

void Hub::setNorm(core::calc::eNorm) {
  // ... base::remBg(r);
  // emit ...
}

Hub::SetsInfo Hub::setsInfo() const
  RET_(SetsInfo(currentSets, currentSet, l::sh(base::fp->clone())))

void Hub::filesModified() {
  modelFiles->signalReset();
  mut(*modelDatasets).combineSets();
}

void Hub::sendSetsInfo() {
  emitSetsInfo(setsInfo());
}

void Hub::setMetaChecked(KeyBag::shr bag) {
  str_vec ks;
  for (auto&& key : currentDict().keys)
    if (bag().contains(key))
      ks.add(key);

  if (currentMetaKeys != ks) {
    currentMetaKeys = ks;
    modelDatasets->signalReset();
  }
}

//------------------------------------------------------------------------------
}
// eof
