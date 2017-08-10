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
#include <dev_lib/defs.inc>
#include <dev_lib/io/fio.hpp>
#include <dev_lib/io/log.hpp>
#include <dev_lib/io/path.hpp>
#include <dev_lib/typ/hash.hpp>
#include <qt_lib/dlg_file.hpp>
#include <qt_lib/dlg_msg.hpp>

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

namespace gui {
//------------------------------------------------------------------------------

using rcidx = QModelIndex const&;

dcl_sub_(FileProxyModel, QSortFilterProxyModel)
  int columnCount(rcidx) const;
  QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;
  QVariant data(rcidx, int = Qt::DisplayRole) const;

private:
  mutable l::hash<str,str> memInfo;
dcl_end

int FileProxyModel::columnCount(rcidx) const {
  return 2;
}

QVariant FileProxyModel::headerData(int section, Qt::Orientation o, int role) const {
  if (1 == section && Qt::Horizontal == o && role == Qt::DisplayRole)
    return "Comment";
  return base::headerData(section, o, role);
}

QVariant FileProxyModel::data(rcidx idx, int role) const {
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

Hub::ref Hub::sessionClear() {
  base::clear();
  emitResetFiles();
  RTHIS
}

Hub::ref Hub::sessionLoad(l_io::path path) may_err {
  l_io::ftin file(path);
  path.cwd();

  using core::io::Json;
  auto json = Json::asSelf(Json::loadFrom(file.asStream()));

  base::load(json);
  emitResetFiles();

  RTHIS
}

void Hub::sessionSave(l_io::path path) const may_err {
  l_io::ftout file(path);
  path.cwd();

  base::save().saveTo(file.asStream());
}

Hub::ref Hub::addFiles(str_vec::rc names) {
  if (!names.isEmpty()) {
    l_io::busy __;
    for (l_io::path path : names)
      if (!base::hasFile(path))
        base::addFile(path);

    emitResetFiles();
  }
  RTHIS
}

Hub::ref Hub::addFiles() {
  auto names = l_qt::dlgOpenFiles(&mut(win), "Add files", l_io::path::cwd(),
                "Data files (*.dat *.mar*);;All files (*.*)",
                new FileProxyModel);

  if (!names.isEmpty())
    l_io::path(names.at(0)).dir().cd();

  return addFiles(names);
}

Hub::ref Hub::remFile(uint i) {
  base::remFile(i);
  emitResetFiles();
  RTHIS
}

Hub::ref Hub::activateFile(uint i, bool on) {
  base::activateFile(i, on);
  emit sigFilesActive();
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
  emit sigCorr();
  RTHIS
}

Hub::ref Hub::corrRem() {
  base::remCorrFile();
  emit sigCorr();
  RTHIS
}

Hub::ref Hub::collectDatasetsFromFiles(uint_vec::rc is, l::pint by) {
  base::collectDatasetsFromFiles(is, by);
  emit sigResetDatasets();
  RTHIS
}

Hub::ref Hub::collectDatasetsFromFiles(uint_vec::rc is) {
  collectDatasetsFromFiles(is, groupBy);
  RTHIS
}

Hub::ref Hub::groupDatasetsBy(l::pint by) {
  collectDatasetsFromFiles(collectedFromFiles, by);
  RTHIS
}

Hub::ref Hub::emitResetFiles() {
  modelFiles->signalReset();
  emit sigResetFiles();
  RTHIS
}

//------------------------------------------------------------------------------
}
// eof
