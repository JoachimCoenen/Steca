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

#include <core/io/io.hpp>
#include <c2/c/c_cpp>
#include <c2/qt/def.hpp>
#include <QFileInfo>
#include <QDir>

namespace core {
//------------------------------------------------------------------------------

TEST("caress",
  auto file = QFileInfo(__FILE__).dir().path() + "/test_caress.dat";
  CHECK_NOTHROW(io::loadCaress(toStr(file)));
)

//------------------------------------------------------------------------------
}
// eof