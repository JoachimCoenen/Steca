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
#include <lib/qt/tabs.hpp>
#include <lib/qt/inc/defs.inc>
#include <lib/qt/lbl.hpp>

namespace gui {
//------------------------------------------------------------------------------

PanelSetup::PanelSetup() : base("") {
  auto tabs = new l_qt::tabs;
  vb().add(tabs);


  {
    tabs->addTab(tabGeometry    = new Panel(), "Geometry");
    auto&& gr = tabGeometry->gr();

    gr.addSection("detector", 3);
    gr.add({new l_qt::lbl("distan"), new l_qt::lbl("disance"), new l_qt::lbl("distance")});
    gr.add({new l_qt::lbl("disce"), new l_qt::lbl("distance"), new l_qt::lbl("distance")});
    gr.add({new l_qt::lbl("tace"), new l_qt::lbl("disce"), new l_qt::lbl("die")});
//      auto&& img = vb.add(new Panel("image"));
//      auto&& hb = img.hb();

    gr.addColStretch();
    gr.addRowStretch();
  }

  {
    tabs->addTab(tabBackground  = new Panel(), "Background");

  }

  {
    tabs->addTab(tabReflections = new Panel(), "Reflections");

  }
}

//------------------------------------------------------------------------------
}
// eof
