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

#ifndef GUI_PANEL_DIFFRACTOGRAM_HPP
#define GUI_PANEL_DIFFRACTOGRAM_HPP

#include "panel.hpp"

namespace gui {
//------------------------------------------------------------------------------

dcl_struct_sub (PanelDiffractogram, Panel)
  PanelDiffractogram(Hub&);

_private
  Panel *tab;
dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
