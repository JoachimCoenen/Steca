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

#include <dev_lib/defs.hpp>
#include <qt_lib/hub.hpp>
#include <core/session.hpp>
#include "acts.hpp"

namespace gui {
//------------------------------------------------------------------------------

struct Win;

dcl_sub2_(Hub, l_qt::Hub, core::Session)
  ref_(Win,  win);
  atr_(Acts, acts);

  Hub(Win&);

  set_(addFiles, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
