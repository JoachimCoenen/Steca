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

#include "panel.h"
#include <app_lib/typ_qt/lst.h>
#include <app_lib/typ_qt/model.h>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(PanelFiles, Panel)
  PanelFiles(Hub&);
 ~PanelFiles();

private:
  Panel *tab;
  l_qt::lst_view  *view;
  l_qt::lst_model *model; // TODO to hub
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
