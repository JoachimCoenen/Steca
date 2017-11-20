/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

#include "dlg_dialog.hpp"
#include <lib/qt/defs.inc>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------
using namespace l_qt::make_widgets;

GroupReflection::GroupReflection() : base("Reflection") {
  vb().add(cbo()).addStretch();
}

//------------------------------------------------------------------------------

GroupGammaSlices::GroupGammaSlices() : base("Gamma slices") {
  gr().add({lbl("count"),   spinUint(5)})
      .add({lbl("degrees"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

GroupGammaRange::GroupGammaRange() : base("Gamma range") {
  gr().add({chk("limit")})
      .add({lbl("min"), spinReal(5, 2)})
      .add({lbl("max"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

GroupPoints::GroupPoints() : base("Points") {
  vb().add(rio("calculated")).add(rio("interpolated")).addStretch();
}

//------------------------------------------------------------------------------

GroupInterpolation::GroupInterpolation() : base("Interpolation") {
  gr().add({lbl("step α"), spinReal(5, 2), lbl("avg. α max"), spinReal(5, 2)})
      .add({lbl("β"), spinReal(5, 2), lbl("radius"), spinReal(5, 2)})
      .add({lbl("idw radius"), spinReal(5, 2), lbl("inclusion %"), spinReal(5, 2)})
      .addRowStretch();
}

//------------------------------------------------------------------------------

GroupDiagram::GroupDiagram() : base("Diagram") {
  gr().add({lbl("y"), cbo(), lbl("x"), cbo()})
      .addRowStretch();
}

//------------------------------------------------------------------------------

TabPoints::ShowCols::ShowCols() {}

TabPoints::TabPoints() {}

//------------------------------------------------------------------------------

TabSave::TabSave() {}

//------------------------------------------------------------------------------

Frame::Frame(strc title, QWidget* parent, Hub::rc hub)
: base(parent, Qt::Dialog), sets(), fp()
, groups(new l_qt::hbox), tabs(new l_qt::tabs), btns(new l_qt::hbox)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  setWindowTitle(l_qt::toQt(title));

  auto&& info = hub.setsInfo();
  mut(sets) = info.sets;
  mut(fp)   = info.fp;

  auto&& vb = new l_qt::vbox();
  setLayout(vb);
  vb->add(groups).add(tabs).add(btns);

  auto&& closeBtn = pushbtn("Close");
  btns->add(closeBtn).addStretch();

  vb->setStretch(1, 1);

  closeBtn->onClick([this]() { close(); });
}

//------------------------------------------------------------------------------
}}
// eof
