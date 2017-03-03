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

#include "session.h"
#include <c2/cpp/async.hpp>
#include <chrono>
#include <c2/c/c_cpp>

core_Session::core_Session() : dummy(0) {}

namespace core {
//------------------------------------------------------------------------------

Session::Session() {
}

int Session::long_square(int i) const {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  return i*i;
}

//------------------------------------------------------------------------------
}
// eof