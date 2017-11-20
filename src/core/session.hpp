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
 * a session object - the state of the various settings
 */

#pragma once

#include "data/files.hpp"
#include "io/json.hpp"
#include "typ/curve.hpp"
#include "typ/geometry.hpp"
#include <lib/dev/inc/num.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/cache.hpp>

namespace core {
//------------------------------------------------------------------------------

extern str_vec const normStrLst;

dcl_(Session)
  using Range  = core::Range;
  using Ranges = core::Ranges;

  Session();

  mut_(clear, ());
  mut_(load, (io::Json::rc)) may_err;
  mth_(io::Json, save, ());

  /// the one active set of loaded files; immutable! - the hub clones it if it needs to be changed
  atr_(l::scoped<data::Files>,     files);
  /// the optional correction file
  atr_(l::shp<data::File>,         corrFile);
  /// the active fitting parameters
  atr_(l::scoped<calc::FitParams>, fp);

  /// add more files
  bol_mut_(addFiles,   (l_io::path_vec::rc)) may_err;
  /// remove files
  bol_mut_(remFilesAt, (uint_vec::rc));

  /// make a file active
  bol_mut_(activateFileAt, (uint, bool));

  /// load a correction file
  mut_(setCorrFile,   (l_io::path::rc)) may_err;
  /// remove the correction file from the session
  mut_(remCorrFile, ());
  /// enable correction (if correction file is loaded)
  mut_(tryEnableCorr, (bool on));

  /// set background ranges
  mut_(setBg, (Ranges::rc));
  /// add a range to the background
  mut_(addBg, (Range::rc));
  /// remove range from the background
  mut_(remBg, (Range::rc));

  /// set the reflection range
  mut_(setRefl, (Range::rc));

  /// set or check the image size
  set_(setImageSize,  (l::sz2)) may_err;

private:
  mut_(updateImageSize, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
