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

#include "panel_image.hpp"
#include <lib/qt/tabs.hpp>
#include <lib/qt/inc/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

PanelImage::PanelImage(Hub& hub) : base("") {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  tabs->addTab((tabImage      = new Panel()), "Image");
  tabs->addTab((tabCorrection = new Panel()), "Correction");
}

//------------------------------------------------------------------------------
}
// eof
