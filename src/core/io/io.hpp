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
 * Load data from Caress or Mar files, or from a set of TIFF images
 */
#pragma once

#include <lib/dev/io/path.hpp>
#include "../data/files.hpp"

namespace core { namespace io {
//------------------------------------------------------------------------------

/// peek into the file; does it look like Caress ?
bool couldBeCaress (l_io::path::rc) may_err;
/// peek into the file; does it look like Mar ?
bool couldBeMar    (l_io::path::rc) may_err;
/// peek into the file; does it look like tiff-listing file ?
bool couldBeTiffDat(l_io::path::rc) may_err;

/// load a file; file type will be sensed
l::own<data::File> load(l_io::path::rc) may_err;

/// load a Caress file (implemented in io_caress.cpp)
l::own<data::File> loadCaress (l_io::path::rc) may_err;
/// load a Caress file comment (implemented in io_caress.cpp)
str loadCaressComment(l_io::path::rc) may_err;
/// load a Mar file (implemented in io_mar.cpp)
l::own<data::File> loadMar    (l_io::path::rc) may_err;
/// load a set of Tiff files (implemented in io_tiff.cpp)
l::own<data::File> loadTiffDat(l_io::path::rc) may_err;

//------------------------------------------------------------------------------
}}
// eof
