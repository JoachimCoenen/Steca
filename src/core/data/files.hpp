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
#include <lib/dev/io/path.hpp>

namespace core { namespace data {
//------------------------------------------------------------------------------

struct Files;

dcl_(File) SHARED  // one file
  ref_(Files,      files);    // parent
  atr_(FileIdx::sh,idx);      // the number within files, 0 = not
  atr_(bool,       isActive); // included in calculations
  atr_(l_io::path, path);
  atr_(str,        name);
  atr_(str,        comment);
  atr_(Sets,       sets);

  File(Files const&, l_io::path::rc);

  set_(addSet, (Set::sh)) may_err;
  mth_(l::sz2, imageSize, ());
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Files, l::vec<File::sh>) SHARED // the whole file group
  atr_(MetaDict::sh, dict);

  Files();

  set_(addFile, (data::File::sh));
  set_(remFile, (uint));
dcl_end

//------------------------------------------------------------------------------
}}
