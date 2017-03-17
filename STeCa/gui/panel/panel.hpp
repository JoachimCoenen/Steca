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

#ifndef GUI_PANEL_HPP
#define GUI_PANEL_HPP

#include <c2/gui_qt/panel.hpp>
#include <c2/gui_qt/layout.hpp>

namespace gui {
//------------------------------------------------------------------------------

struct Hub;

def_struct_sub (Panel, c_qt::panel)
  Panel(Hub&);
  Panel(c::strc title, Hub&);

protected:
  Hub& hub;
public:
  c_gui::box &vb;
def_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
