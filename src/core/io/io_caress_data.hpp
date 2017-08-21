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
#include <lib/dev/defs.hpp>
#include <lib/dev/typ/buf.hpp>

namespace core { namespace io {
//------------------------------------------------------------------------------

bool openFile(strc);
void closeFile();

enum dtype {
  NONE, INT16, INT32, FLT32, CHR
};

using buf = l::buf;

buf getData(dtype, uint n);
str getString(uint n);
str getAsString(dtype, uint n);
float  getAsFloat(dtype, uint n);

l::vec<int32> getAdet(dtype, uint n);

bool nextDataUnit(str& elem, str& node, dtype&, uint& n);

//------------------------------------------------------------------------------
}}
