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

#include "io.hpp"
#include <c2/qt/qstr.hpp>
// TODO do w/o Qt and move to core or at a level with core?
#include <QByteArray>
#include <QFileInfo>
#include <QFile>
#include <QSortFilterProxyModel>
// also remove Qt5 Widgets from CMakeLists.txt
#include <QFileSystemModel>
#include <c2/inc/c_cpp>

namespace core { namespace io {
//------------------------------------------------------------------------------

// peek at up to maxLen bytes (to establish the file type)
static QByteArray peek(uint pos, uint maxLen, c::path::rc path) {
  QFile file(path.p);

  if (file.open(QFile::ReadOnly) && file.seek(pos))
    return file.read(maxLen);

  return QByteArray();
}

bool couldBeCaress(c::path::rc path) {
  static QByteArray const header("\020\012DEFCMD DAT");
  return header == peek(0, c::to_uint(header.size()), path);
}

// Mar file format
bool couldBeMar(c::path::rc path) {
  static QByteArray const header("mar research");
  return header == peek(0x80, c::to_uint(header.size()), path);
}

// Text .dat file with metadata for tiff files
bool couldBeTiffDat(c::path::rc path) {
  QFile file(path.p);

  if (!file.open(QFile::ReadOnly))
    return false;

  bool couldBe = false;

  QByteArray line;

  while (!(line = file.readLine()).isEmpty()) {
    QString s = line;

    int commentPos = s.indexOf(';');
    if (commentPos >= 0)
      s = s.left(commentPos);

    if ((s = s.simplified()).isEmpty())
      continue;

    auto lst = s.split(' ');
    int cnt = lst.count();
    if (cnt < 2 || cnt > 4)
      return false;

    couldBe = true;
  }

  return couldBe;
}


//------------------------------------------------------------------------------

data::File::sh load(data::Files& files, c::path::rc path) may_err {
  check_or_err (QFileInfo(path.p).exists(), "File does not exist: ", path);

  data::File::sh file;

  if (couldBeCaress(path))
    file = loadCaress(files, path);
  else if (couldBeMar(path))
    file = loadMar(files, path);
  else if (couldBeTiffDat(path))
    file = loadTiffDat(files, path);
  else
    c::err("unknown file type: ", path);

  check_or_err (file->sets.size() > 0,
                "File contains no datasets: ", path);

  // ensure that all datasets have images of the same size
  auto size = file->sets.first()->image->size();
  for (auto& set : file->sets)
    if (set->image->size() != size)
      c::err("Inconsistent image size in file: ", path);

  return file;
}

//------------------------------------------------------------------------------

typedef QModelIndex const& rcidx;

dcl_struct_sub_(ProxyModel, QSortFilterProxyModel)
  int columnCount(rcidx) const;
  QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;
  QVariant data(rcidx, int = Qt::DisplayRole) const;
dcl_end

int ProxyModel::columnCount(rcidx) const {
  return 2;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation o, int role) const {
  if (1 == section && Qt::Horizontal == o && role == Qt::DisplayRole)
    return "Comment";
  return base::headerData(section, o, role);
}

QVariant ProxyModel::data(rcidx idx, int role) const {
  if (idx.isValid() && 1 == idx.column()) {
    if (Qt::DisplayRole == role) {
      QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(sourceModel());
      auto ix0 = fileModel->index(mapToSource(idx).row(), 0, mapToSource(idx.parent()));
      QFileInfo info(fileModel->rootDirectory().filePath(fileModel->fileName(ix0)));
      if (info.isFile()) {
        c::str path = toStr(info.absoluteFilePath());
        if (io::couldBeCaress(path))
          return "[car] " + QString(loadCaressComment(path).p);
        else if (couldBeMar(path))
          return "[mar] ";
        else if (couldBeTiffDat(path))
          return "[tif] ";
      }
    }

    return QVariant();
  }

  return base::data(idx, role);
}

//------------------------------------------------------------------------------
}}
// eof
