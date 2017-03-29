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

#ifndef CORE_IO_CARESS_DATA_HPP
#define CORE_IO_CARESS_DATA_HPP

#include <c2/inc/c_def.h>
#include <c2/c/mem.h>
#include <c2/c/str.h>
#include <c2/cpp/vec.hpp>

namespace core { namespace io {
//------------------------------------------------------------------------------

bool openFile(c::strc);
void closeFile();

enum dtype {
  NONE, INT16, INT32, FLT32, STR
};

c::mem getData(uint n, sz_t, dtype);
c::str getString(uint n);
c::str getAsString(uint n, dtype);
float  getAsFloat(uint n, dtype);

bool nextDataUnit(c::str& elem, c::str& node, dtype&, uint& n);

//------------------------------------------------------------------------------
}}
#endif