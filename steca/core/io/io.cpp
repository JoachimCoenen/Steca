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
#include "io_tiff.hpp"
#include <dev_lib/defs.inc>
#include <dev_lib/io/fin.hpp>
#include <algorithm>

namespace core { namespace io {
//------------------------------------------------------------------------------

// Caress format
bool couldBeCaress(l_io::path::rc path) {
  try {
    l::buf header("\020\012DEFCMD DAT");
    return header == l_io::fbin(path).read(header.size());
  } catch(l::exc::rc) {
    return false;
  }
}

// Mar format
bool couldBeMar(l_io::path::rc path) {
  try {
    l::buf header("mar research");
    return header == l_io::fbin(path).seek(0x80).read(header.size());
  } catch(l::exc::rc) {
    return false;
  }
}

// Text .dat file with metadata for tiff files
bool couldBeTiffDat(l_io::path::rc path) {
  bool couldBe = false;

  try {
    FileTiffDat fin(path);
    while (!couldBe && fin.hasMore()) {
      auto row = fin.getRow();
      EXPECT_(row.size() > 0)
      str fileName = row.at(0);

      // naive detection: possibly contains a name ending in .tif, .tiff, .TIFF etc.
      std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
      auto pos = fileName.find_first_of(".tif");
      if (str::npos != pos)
        couldBe = true;
    }
  } catch(l::exc::rc) {
    return false;
  }

  return couldBe;
}

//------------------------------------------------------------------------------

data::File::sh load(data::Files& files, l_io::path::rc path) may_err {
  data::File::sh file;

  if (couldBeCaress(path))
    file = loadCaress(files, path);
  else if (couldBeMar(path))
    file = loadMar(files, path);
  else if (couldBeTiffDat(path))
    file = loadTiffDat(files, path);
  else
    l::err(CAT("unknown file type: ", path));

  check_or_err_(file->sets.size() > 0,
                CAT("File contains no datasets: ", path));

  // ensure that all datasets have images of the same size
  auto size = file->sets.first()->image->size();
  for (auto& set : file->sets)
    if (set->image->size() != size)
      l::err(CAT("Inconsistent image size in file: ", path));

  return file;
}

//------------------------------------------------------------------------------

//typedef QModelIndex const& rcidx;

//dcl_sub_(ProxyModel, QSortFilterProxyModel)
//  int columnCount(rcidx) const;
//  QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;
//  QVariant data(rcidx, int = Qt::DisplayRole) const;
//dcl_end

//int ProxyModel::columnCount(rcidx) const {
//  return 2;
//}

//QVariant ProxyModel::headerData(int section, Qt::Orientation o, int role) const {
//  if (1 == section && Qt::Horizontal == o && role == Qt::DisplayRole)
//    return "Comment";
//  return base::headerData(section, o, role);
//}

//// TODO move to GUI, remove Qt modules from CMakeLists.txt
//QVariant ProxyModel::data(rcidx idx, int role) const {
//  if (idx.isValid() && 1 == idx.column()) {
//    if (Qt::DisplayRole == role) {
//      QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(sourceModel());
//      auto ix0 = fileModel->index(mapToSource(idx).row(), 0, mapToSource(idx.parent()));
//      QFileInfo info(fileModel->rootDirectory().filePath(fileModel->fileName(ix0)));
//      if (info.isFile()) {
//        auto path = l_qt::fromQt(info.absoluteFilePath());
//        if (io::couldBeCaress(path))
//          return QString("[car] %1").arg(loadCaressComment(path).c_str());
//        else if (couldBeMar(path))
//          return "[mar] ";
//        else if (couldBeTiffDat(path))
//          return "[tif] ";
//      }
//    }

//    return QVariant();
//  }

//  return base::data(idx, role);
//}

//------------------------------------------------------------------------------
}}
// eof
