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
#include <lib/dev/io/path.hpp>
#include "../data/files.hpp"

namespace core { namespace io {
//------------------------------------------------------------------------------

bool couldBeCaress (l_io::path::rc) may_err;
bool couldBeMar    (l_io::path::rc) may_err;
bool couldBeTiffDat(l_io::path::rc) may_err;

// load a file; file type will be sensed
l::own<data::File> load(l_io::path::rc) may_err;

// load a specific file type
l::own<data::File> loadCaress (l_io::path::rc) may_err;
l::own<data::File> loadMar    (l_io::path::rc) may_err;
l::own<data::File> loadTiffDat(l_io::path::rc) may_err;

str loadCaressComment(l_io::path::rc) may_err;

//------------------------------------------------------------------------------
}}
