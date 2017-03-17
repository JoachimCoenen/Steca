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

#ifndef GUI_PANEL_FILES_HPP
#define GUI_PANEL_FILES_HPP

#include "panel.hpp"
#include <c2/gui_qt/lst.hpp>
#include <c2/gui_qt/model.hpp>

namespace gui {
//------------------------------------------------------------------------------

def_struct_sub (PanelFiles, Panel)
  PanelFiles(Hub&);
 ~PanelFiles();

private:
  Panel *tab;
  c_qt::lst_view  *view;
  c_qt::lst_model *model; // TODO to hub
def_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
