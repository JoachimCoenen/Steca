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

#include "panel_diffractogram.hpp"
#include "../hub.hpp"
#include <c2/gui_qt/inc.inc>

namespace gui {
//------------------------------------------------------------------------------

PanelDiffractogram::PanelDiffractogram(Hub& hub_) : base("", hub_) {
  vb.add(new c_qt::lbl("<Diffractogram>"));
  auto &hb = vb.hb();

  hb.add(new c_qt::lbl("norm:"));
  hb.add(new c_qt::cbo());
  hb.add(new c_qt::lbl(" inten:"));
  hb.add(new c_qt::rio("sum"));
  hb.add(new c_qt::rio("avg ×"));
  hb.add(new c_qt::spin()); // em4_2, 0.001 setDecimals(3);

  hb.addStretch();
  hb.add(new c_qt::actbtn(hub.acts.get(hub.acts.DIFF_ZOOM)));
  hb.addStretch();

  hb.add(new c_qt::chk(hub.acts.get(hub.acts.DIFF_ALL_DSETS)));
  hb.add(new c_qt::chk(hub.acts.get(hub.acts.DIFF_FIX_INTEN)));
}

//------------------------------------------------------------------------------
}
// eof
