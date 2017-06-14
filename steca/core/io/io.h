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

#ifndef CORE_IO_HPP
#define CORE_IO_HPP

#include <core/data/data.h>
#include <dev_lib/typ/path.h>

namespace core { namespace io {
//------------------------------------------------------------------------------

bool couldBeCaress(l::path::rc);
bool couldBeMar(l::path::rc);
bool couldBeTiffDat(l::path::rc);

// load a file; file type will be sensed
data::File::sh load(data::Files&, l::path::rc) may_err;

// load a specific file type
data::File::sh loadCaress(data::Files&, l::path::rc)  may_err;
data::File::sh loadMar(data::Files&, l::path::rc)     may_err;
data::File::sh loadTiffDat(data::Files&, l::path::rc) may_err;

str loadCaressComment(l::path::rc) may_err;

//------------------------------------------------------------------------------
}}
#endif