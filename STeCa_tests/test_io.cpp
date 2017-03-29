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

#include <c2/gui_qt/def.hpp>
#include <core/io/io.hpp>
#include <c2/inc/c_cpp>
#include <QDir>
#include <QFileInfo>

namespace core {
//------------------------------------------------------------------------------

TEST("caress",
  auto file = QFileInfo(__FILE__).dir().path() + "/test_caress.dat";
//  auto file = "/home/jan/C/+dev/fz/data/test/m200093580.dat";

  data::Files files;
  CHECK_NOTHROW(io::loadCaress(files, toStr(file)));
)

//------------------------------------------------------------------------------
}
// eof
