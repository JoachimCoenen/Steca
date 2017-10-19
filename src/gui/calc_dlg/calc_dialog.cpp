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

#include "calc_dialog.hpp"
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------
using namespace l_qt::make_widgets;

PanelReflection::PanelReflection() {
  vb().add(cbo()).addStretch();
}

//------------------------------------------------------------------------------

PanelGammaSlices::PanelGammaSlices() {
  gr().add({lbl("count"),   spinUint(5)})
      .add({lbl("degrees"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

PanelGammaRange::PanelGammaRange() {
  gr().add({chk("limit")})
      .add({lbl("min"), spinReal(5, 2)})
      .add({lbl("max"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

PanelPoints::PanelPoints() {
  vb().add(rio("calculated")).add(rio("interpolated")).addStretch();
}

//------------------------------------------------------------------------------

PanelInterpolation::PanelInterpolation() {
  gr().add({lbl("step α"), spinReal(5, 2), lbl("avg. α max"), spinReal(5, 2)})
      .add({lbl("β"), spinReal(5, 2), lbl("radius"), spinReal(5, 2)})
      .add({lbl("idw radius"), spinReal(5, 2), lbl("inclusion %"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

PanelDiagram::PanelDiagram() {
  gr().add({lbl("y"), cbo(), lbl("x"), cbo()})
      .addRowStretch();
}

//------------------------------------------------------------------------------

Frame::Frame(strc title, QWidget* parent, Hub::rc hub)
: base(parent, Qt::Dialog), sets(), fp() {
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  setWindowTitle(l_qt::toQt(title));

  auto&& info = hub.setsInfo();
  mut(sets) = info.sets;
  mut(fp)   = info.fp;
}

//------------------------------------------------------------------------------
}}
// eof
