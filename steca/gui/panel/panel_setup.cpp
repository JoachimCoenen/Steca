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

#include "panel_setup.hpp"
#include <qt_lib/tabs.hpp>
#include <qt_lib/inc/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

PanelSetup::PanelSetup(Hub& hub_) : base("", hub_) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  tabs->addTab((tabGeometry    = new Panel(hub)), "Geometry");
  tabs->addTab((tabBackground  = new Panel(hub)), "Background");
  tabs->addTab((tabReflections = new Panel(hub)), "Reflections");
}

//------------------------------------------------------------------------------
}
// eof
