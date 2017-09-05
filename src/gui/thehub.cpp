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
, modelFiles(new ModelFiles(*this))
, modelDatasets(new ModelDatasets(*this))
, modelMetadata(new ModelMetadata(*this)) {}

Hub::~Hub() {
  delete modelFiles;
}

void Hub::sessionClear() {
  base::clear();
  emitFiles(files);
}

Hub::ref Hub::sessionLoad(l_io::path path) may_err {
  l_io::ftin file(path);
  path.cwd();

  using core::io::Json;
  auto json = Json::asSelf(Json::loadFrom(file.asStream()));

  base::load(json);
  emitFiles(files);

  RTHIS
}

void Hub::sessionSave(l_io::path path) const may_err {
  l_io::ftout file(path);
  path.cwd();

  base::save().saveTo(file.asStream());
}

void Hub::addFiles(l_io::path_vec::rc ps) emits {
  if (base::addFiles(ps))
    emitFiles(files);
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
    emitFiles(files);
}

Hub::ref Hub::activateFileAt(uint i, bool on) {
  base::activateFileAt(i, on);
  emitFilesActive();
  RTHIS
}

Hub::ref Hub::selectFileAt(int i) {
  emitFileSelected(i < 0
    ? core::data::File::sh()
    : files->at(l::to_u(i)));
  RTHIS
}

Hub::ref Hub::activateDatasetAt(uint i, bool on) {
  base::activateDatasetAt(i, on);
  emitDatasetsActive();
  RTHIS
}

Hub::ref Hub::selectDatasetAt(int i) {
  emitDatasetSelected(i < 0
    ? core::data::CombinedSet::sh()
    : collectedDatasets.at(l::to_u(i)));
  RTHIS
}

Hub::ref Hub::corrEnable(bool on) {
  if (on && !corrFile) {
    str name = l_qt::dlgOpenFile(&mut(win), "Select correction file", l_io::path::cwd(),
                 "Data files (*.dat *.mar*);;All files (*.*)");
    if (!name.isEmpty()) {
      l_io::path(name).dir().cd();
      base::setCorrFile(name);
    }
  }

  base::tryEnableCorr(on);
  emitCorr();
  RTHIS
}

Hub::ref Hub::corrRem() {
  base::remCorrFile();
  emitCorr();
  RTHIS
}

Hub::ref Hub::collectDatasetsFromFiles(uint_vec::rc is, l::pint by) {
  base::collectDatasetsFromFiles(is, by);
  emitDatasetsReset();
  RTHIS
}

Hub::ref Hub::collectDatasetsFromFiles(uint_vec::rc is) {
  collectDatasetsFromFiles(is, groupedBy());
  RTHIS
}

Hub::ref Hub::groupDatasetsBy(l::pint by) {
  collectDatasetsFromFiles(collectedFromFiles, by);
  RTHIS
}

uint Hub::dictSize() const {
  return files->dict->size();
}

str Hub::dictKey(uint i) const {
  return files->dict->key(i);
}

bool Hub::dictChecked(uint i) const {
  auto&& dict = *files->dict;
  return dict.checked(dict.key(i));
}

Hub::ref Hub::dictCheck(uint i, bool on) {
  auto&& dict = *files->dict;
  mut(dict).check(dict.key(i), on);
  RTHIS
}

void Hub::emitFiles(core::data::Files::sh sh) const {
  emit sigFiles(sh);
}

void Hub::emitFilesActive() const {
  emit sigFilesActive();
}

void Hub::emitFileSelected(core::data::File::sh sh) const {
  emit sigFileSelected(sh);
}

void Hub::emitCorr() const {
  emit sigCorr();
}

void Hub::emitDatasetsReset() const {
  emit sigDatasetsReset();
}

void Hub::emitDatasetsActive() const {
  emit sigDatasetsActive();
}

void Hub::emitDatasetSelected(core::data::CombinedSet::sh sh) const {
  emit sigDatasetSelected(sh);
}

//------------------------------------------------------------------------------
}
// eof
