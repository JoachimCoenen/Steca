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
 * Settings.
 */
#pragma once

#include <lib/qt/defs.hpp>
#include <lib/qt/app/settings.hpp>

namespace gui {
//------------------------------------------------------------------------------

/// startup-actions configuration
dcl_sub_(Settings, l_qt::Settings)
  using base::base;

  cst_(pcstr, GROUP_CONFIG);

  cst_(pcstr, STARTUP_ABOUT);
  cst_(pcstr, STARTUP_UPDATE);
  cst_(pcstr, CURRENT_VERSION);
dcl_end

//------------------------------------------------------------------------------
}
// eof
