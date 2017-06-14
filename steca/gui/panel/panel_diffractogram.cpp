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

#include "panel_diffractogram.h"
#include "../hub.h"
#include <qt_lib/wgt_inc.h>

namespace gui {
//------------------------------------------------------------------------------

PanelDiffractogram::PanelDiffractogram(Hub& hub_) : base("", hub_) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab((tab = new Panel(hub)), "Diffractogram");

  auto &hb = tab->vb.hb();

  hb.add(new l_qt::lbl("norm:"));
  hb.add(new l_qt::cbo(hub.normStrLst));
  hb.add(new l_qt::lbl(" inten:"));
  hb.add(new l_qt::rio("sum"));
  hb.add(new l_qt::rio("avg ×"));
  hb.add(new l_qt::spin()); // em4_2, 0.001 setDecimals(3);

  hb.addStretch();
  hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.DIFF_ZOOM)));
  hb.addStretch();

  hb.add(new l_qt::chk(hub.acts.get(hub.acts.DIFF_ALL_DSETS)));
  hb.add(new l_qt::chk(hub.acts.get(hub.acts.DIFF_FIX_INTEN)));
}

//------------------------------------------------------------------------------
}
// eof
