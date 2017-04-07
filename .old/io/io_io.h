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

#ifndef IO_IO_H
#define IO_IO_H

#include "data/data_file.h"

namespace io {
//------------------------------------------------------------------------------

bool couldBeCaress(QFileInfo const&);
bool couldBeMar(QFileInfo const&);
bool couldBeTiffDat(QFileInfo const&);

// load a file; file type will be sensed
data::shp_File load(rcstr filePath) THROWS;

// load a Caress file
data::shp_File loadCaress(rcstr filePath) THROWS;
data::shp_File loadCaress2(rcstr filePath) THROWS;
str loadCaressComment(rcstr filePath);

// load a Mar file
data::shp_File loadMar(rcstr filePath) THROWS;

// load a metadata file + tiffs
data::shp_File loadTiffDat(rcstr filePath) THROWS;

//------------------------------------------------------------------------------
}
#endif // IO_IO_H
