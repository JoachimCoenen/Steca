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

#include "panel_setup.hpp"
#include <lib/qt/tabs.hpp>
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"
#include <core/calc/reflection.hpp>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ViewReflections, l_qt::lst_view)
  ViewReflections(ModelReflections&);
dcl_end

ViewReflections::ViewReflections(ModelReflections& model) {
  setModel(&model);
}

//------------------------------------------------------------------------------

PanelSetup::PanelSetup(Hub& hub) : base("") {
  using namespace l_qt::make_widgets;
  auto&& as = hub.acts;

  auto tabs = new l_qt::tabs;
  vb().add(tabs);

  {
    tabs->addTab(tabGeometry    = new Panel(), "Geometry");
    auto&& vb = tabGeometry->vb();

    auto&& detDist    = spinReal(7, 2);
    auto&& detPixSize = spinReal(7, 3);
    auto&& detBeamX   = spinInt(7);
    auto&& detBeamY   = spinInt(7);

    {
      auto&& gr = vb.gr();
      gr.addSection("detector", 4);
      gr.add({lbl("distance"),    detDist,    lbl("<mm")});
      gr.add({lbl("pixel size"),  detPixSize, lbl("<mm")});
      gr.addSection("beam", 4);
      gr.add({lbl("offset X"),    detBeamX,   lbl("<pix")});
      gr.add({lbl("Y"),           detBeamY,   lbl("<pix")});
      gr.setColumnStretch(3, 1);
    }

    vb.addSection("image");

    {
      auto&& gr = vb.gr();
      gr.add({btn(as.get(as.IMG_ROTATE0)),     lbl("<rotate")});
      gr.add({btn(as.get(as.IMG_MIRROR_HORZ)), lbl("<mirror")});
    }

    auto&& cutLeft   = spinUint(3);
    auto&& cutRight  = spinUint(3);
    auto&& cutTop    = spinUint(3);
    auto&& cutBottom = spinUint(3);

    {
      auto&& gr = vb.gr();
      gr.add({btn(as.get(as.IMG_LINK_CUT)), lbl("cut"),
              ico(":/icon/cutLeft"), cutLeft, ico(":/icon/cutRight"), cutRight});
      gr.add({nullptr, nullptr,
              ico(":/icon/cutTop"), cutTop, ico(":/icon/cutBottom"), cutBottom});
      gr.addStretch();
    }

    vb.addStretch();
  }

  {
    tabs->addTab(tabBackground  = new Panel(), "Background");
    auto&& vb = tabBackground->vb();

    auto&& polDegree = spinUint(1); polDegree->max(4);

    vb.hb().add(btn(as.get(as.BG_SELECT)))
           .add(btn(as.get(as.BG_SHOW)))
           .add(btn(as.get(as.BG_CLEAR)))
           .addStretch();
    vb.hb().add(lbl("Pol. degree")).add(polDegree)
           .addStretch();

    vb.addStretch();
  }

  {
    tabs->addTab(tabReflections = new Panel(), "Reflections");
    auto&& vb = tabReflections->vb();

    vb.hb()
      .add(btn(as.get(as.REFL_SELECT)))
      .add(btn(as.get(as.BG_SHOW)))
      .add(btn(as.get(as.REFL_CLEAR)))
      .addStretch();

    auto&& view = new ViewReflections(modelReflections);

    vb.add(view);

    auto&& cb = cbo(core::fit::PeakFun::sTypes);
    vb.hb()
      .add(cb)
      .add(btn(as.get(as.REFL_ADD))).add(btn(as.get(as.REFL_REM)))
      .addStretch();

    vb.gr()
      .addSection("guess", 4)
      .add({lbl("min x"), spinReal(6, 2), lbl("y"), spinReal(6, 2)})
      .add({lbl("fwhm"),  spinReal(6, 2)})
      .addSection("fit", 4)
      .add({lbl("min x"), spinReal(6, 2), lbl("y"), spinReal(6, 2)})
      .add({lbl("fwhm"),  spinReal(6, 2)})
      .addStretch();

    vb.addStretch();
  }
}

//------------------------------------------------------------------------------
}
// eof
