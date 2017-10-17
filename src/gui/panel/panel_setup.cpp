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
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"

namespace gui {
//------------------------------------------------------------------------------

PanelSetup::PanelSetup(Hub& hub) : base("") {
  auto tabs = new l_qt::tabs;
  vb().add(tabs);

  {
    tabs->addTab(tabGeometry    = new Panel(), "Geometry");
    auto&& vb = tabGeometry->vb();

    vb.addSection("detector");

    auto&& detDist    = new l_qt::spinReal();
    auto&& detPixSize = new l_qt::spinReal(3);
    auto&& detBeamX   = new l_qt::spinInt();
    auto&& detBeamY   = new l_qt::spinInt();

    {
      auto&& gr = vb.gr();
      gr.add({new l_qt::lbl("distance"),      detDist,    new l_qt::lbl("mm")});
      gr.add({new l_qt::lbl("pixel size"),    detPixSize, new l_qt::lbl("mm")});
      gr.add({new l_qt::lbl("beam offset X"), detBeamX,   new l_qt::lbl("pix")});
      gr.add({new l_qt::lbl("Y"),             detBeamY,   new l_qt::lbl("pix")});
      gr.addStretch();
    }

    vb.addSection("image");

    {
      auto&& gr = vb.gr();
      gr.add({new l_qt::actbtn(hub.acts.get(hub.acts.IMG_ROTATE0)),     new l_qt::lbl("rotate")});
      gr.add({new l_qt::actbtn(hub.acts.get(hub.acts.IMG_MIRROR_HORZ)), new l_qt::lbl("mirror")});
    }

    auto&& cutLeft   = new l_qt::spinUint(3);
    auto&& cutRight  = new l_qt::spinUint(3);
    auto&& cutTop    = new l_qt::spinUint(3);
    auto&& cutBottom = new l_qt::spinUint(3);

    {
      auto&& gr = vb.gr();
      gr.add({new l_qt::actbtn(hub.acts.get(hub.acts.IMG_LINK_CUT)), new l_qt::lbl("cut"),
              new l_qt::ico(":/icon/cutLeft"), cutLeft, new l_qt::ico(":/icon/cutRight"), cutRight});
      gr.add({nullptr, nullptr,
              new l_qt::ico(":/icon/cutTop"), cutTop, new l_qt::ico(":/icon/cutBottom"), cutBottom});
      gr.addStretch();
    }

    vb.addStretch();
  }

  {
    tabs->addTab(tabBackground  = new Panel(), "Background");
    auto&& vb = tabBackground->vb();

    auto&& polDegree = new l_qt::spinUint(1); polDegree->max(4);

    vb.hb().add(new l_qt::actbtn(hub.acts.get(hub.acts.BG_SELECT)))
           .add(new l_qt::actbtn(hub.acts.get(hub.acts.BG_SHOW)))
           .add(new l_qt::actbtn(hub.acts.get(hub.acts.BG_CLEAR)))
           .addStretch();
    vb.hb().add(new l_qt::lbl("Pol. degree")).add(polDegree)
           .addStretch();

    vb.addStretch();
  }

  {
    tabs->addTab(tabReflections = new Panel(), "Reflections");
    auto&& vb = tabReflections->vb();

    vb.hb().add(new l_qt::actbtn(hub.acts.get(hub.acts.REFL_SELECT)))
           .add(new l_qt::actbtn(hub.acts.get(hub.acts.BG_SHOW)))
           .add(new l_qt::actbtn(hub.acts.get(hub.acts.REFL_CLEAR)))
           .addStretch();

    vb.addStretch();
  }
}

//------------------------------------------------------------------------------
}
// eof
