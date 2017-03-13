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

#ifndef GUI_WIN_HPP
#define GUI_WIN_HPP

#include <c2/gui_qt/win.hpp>
#include "hub.hpp"
#include "acts.hpp"

namespace gui {
//------------------------------------------------------------------------------

struct PanelFiles;
struct PanelDatasets;
struct PanelSetup;
struct PanelImage;
struct PanelDiffractogram;
struct PanelMetadata;

def_struct_sub (Win, c_qt::win)
  Win();
  Hub hub; Acts acts;

  PanelFiles         *panelFiles;
  PanelDatasets      *panelDatasets;
  PanelSetup         *panelSetup;
  PanelImage         *panelImage;
  PanelDiffractogram *panelDiffractogram;
  PanelMetadata      *panelMetadata;
def_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
