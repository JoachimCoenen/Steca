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

#pragma once
#include "../typ/def.hpp"
#include "sets.hpp"

namespace core { namespace data {
//------------------------------------------------------------------------------

struct Files;

dcl_(File) SHARED  // one file
  atr_(FileSrc::sh,src);      // the number within files, 0 = not
  atr_(bool,       isActive); // included in calculations
  atr_(Sets,       sets);
  atr_(MetaDict::sh, dict); // for all sets

  File(l_io::path::rc);

  set_(addSet, (Set::sh)) may_err;
  mth_(l::sz2, imageSize, ());
dcl_end

//------------------------------------------------------------------------------

// the whole file group
dcl_reimpl_(Files, l::vec<File::sh>) SHARED CLONED
  using base::begin;
  using base::end;
  using base::size;
  using base::at;

  atr_(FilesMetaDict::sh, dict);

  Files();
  virtual ~Files() {}

  bol_(hasPath, (l_io::path::rc));
  voi_mut_(addFile, (data::File::sh));
  voi_mut_(remFileAt, (uint));

  mth_(CombinedSets::sh, collectDatasets, (l::pint groupedBy));

private:
  Files(rc);
dcl_end

//------------------------------------------------------------------------------
}}
