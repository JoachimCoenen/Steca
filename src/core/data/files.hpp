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

/** @file
 * Data files (on the disk and read into memory.
 */
#pragma once

#include "sets.hpp"

namespace core { namespace data {
//------------------------------------------------------------------------------

struct Files;

/** One file with dataset, as read from a disk file
 */
dcl_(File) SHARED CLONED
  /// information about the source (disk) file
  atr_(FileSrc::shp,src);
  /// is it inculded in calculations ?
  atr_(bool,       isActive);
  /// datasets in the file
  atr_(Sets,       sets);
  /// dictionary of metadata; union of metadata types from all sets
  atr_(MetaDict::shp, dict);

  File(l_io::path::rc);

  set_(addSet, (Set::shr)) may_err;
  mth_(l::sz2, imageSize, ());

private:
  File(rc) = default;
dcl_end

//------------------------------------------------------------------------------

/** The group of all loaded files
 */
dcl_reimpl_(Files, l::vec<File::shp>) SHARED CLONED
  UB4_(begin, end, size, at)

  /// shared dictionary
  atr_(FilesMetaDict::shr, dict);

  Files();
  virtual ~Files() {}

  /// is there afile with this path already in the group ?
  bol_(hasPath, (l_io::path::rc));
  /// add a file
  mut_(addFile, (data::File::shp));
  /// remove a file
  mut_(remFileAt, (uint));

  /// collect datasets from all files
  mth_(CombinedSets::shr, collectDatasets, (l::pint groupedBy));

private:
  Files(rc);
dcl_end

//------------------------------------------------------------------------------
}}
// eof
