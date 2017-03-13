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
#include <c2/c/c_cpp>

namespace gui {
//------------------------------------------------------------------------------

Win::Win() : hub(), acts(*this) {
  auto &sp = makeHSplit();

  auto &ls  = sp.vs();  // left
  ls.add(new PanelFiles);
  ls.add(new PanelDatasets);

  auto &cs  = sp.vs();  // center
  auto &ts  = cs.hs();

  ts.add(new PanelSetup);
  ts.add(new PanelImage);
  cs.add(new PanelDiffractogram);

  auto &rs  = sp.vs();  // right
  rs.add(new PanelMetadata);

//  auto &vb = makePanel().makeVBox();

//  vb.add(new c_qt::actbtn(acts.get(acts.QUIT).ptr()));
//  vb.add(new c_qt::actbtn(acts.get(acts.SHOW_FILES).ptr()));
}

//------------------------------------------------------------------------------
}
// eof
