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

#include "win.hpp"
#include "panel/inc.inc"
#include <c2/gui_qt/inc.inc>
#include <c2/inc/c_cpp>

namespace gui {
//------------------------------------------------------------------------------

Win::Win() : hub(*this) {
  panelFiles         = new PanelFiles(hub);
  panelDatasets      = new PanelDatasets(hub);
  panelSetup         = new PanelSetup(hub);
  panelImage         = new PanelImage(hub);
  panelDiffractogram = new PanelDiffractogram(hub);
  panelMetadata      = new PanelMetadata(hub);

  auto &hb = makePanel().makeHBox(); // main: horizontal layout
  auto &tb = hb.vb();   // toolbar

  auto &sp = hb.hs();

  auto &ls  = sp.vs();  // left
  ls.add(panelFiles);
  ls.add(panelDatasets);

  auto &cs  = sp.vs();  // center
  auto &ts  = cs.hs();

  ts.add(panelSetup);
  ts.add(panelImage);
  cs.add(panelDiffractogram);

  auto &rs  = sp.vs();  // right
  rs.add(panelMetadata);

  auto &a = hub.acts;

  tb.add(new c_qt::actbtn(a.get(a.SHOW_FILES)));
  tb.add(new c_qt::actbtn(a.get(a.SHOW_DATASETS)));
  tb.add(new c_qt::actbtn(a.get(a.SHOW_METADATA)));
  tb.addStretch();
  tb.add(new c_qt::actbtn(a.get(a.QUIT)));

  a.get(a.SHOW_FILES).onToggle([this](bool on) {
    panelFiles->show(on);
  }).check();

  a.get(a.SHOW_DATASETS).onToggle([this](bool on) {
    panelDatasets->show(on);
  }).check();

  a.get(a.SHOW_METADATA).onToggle([this](bool on) {
    panelMetadata->show(on);
  }).check();
}

bool Win::onClose() {
  return true; // dlgYes("Quit?");
}

//------------------------------------------------------------------------------
}
// eof
