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

#include <qt_lib/win.hpp>
#include "hub.hpp"

namespace gui {
//------------------------------------------------------------------------------

struct PanelFiles;
struct PanelDatasets;
struct PanelSetup;
struct PanelImage;
struct PanelDiffractogram;
struct PanelMetadata;

dcl_sub_(Win, l_qt::win)
  Win();
  Hub hub;

  act_mut_(about, ());
  mth_mut_(bool, onClose, ());

  PanelFiles         *panelFiles;
  PanelDatasets      *panelDatasets;
  PanelSetup         *panelSetup;
  PanelImage         *panelImage;
  PanelDiffractogram *panelDiffractogram;
  PanelMetadata      *panelMetadata;
dcl_end

//------------------------------------------------------------------------------
}
// eof
