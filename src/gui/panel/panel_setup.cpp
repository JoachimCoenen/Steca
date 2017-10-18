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
//dcl_sub2_(ViewModelBase, RefHub, l_qt::lst_view)
//  ViewModelBase(Hub&);
//dcl_end

//template <typename Model>
//dcl_sub_(ViewModel, ViewModelBase)
//  ViewModel(Hub& hub, Model const* model_) : base(hub) {
//    setModel(model = model_);
//  }

//protected:
//  Model const* model;
//dcl_end

dcl_sub_(ViewReflections, l_qt::lst_view)
  ViewReflections(ModelReflections&);
dcl_end

ViewReflections::ViewReflections(Hub& hub) : base(hub, hub.modelMetadata) {}

//------------------------------------------------------------------------------

PanelSetup::PanelSetup(Hub& hub) : base("") {
  auto tabs = new l_qt::tabs;
  vb().add(tabs);

  {
    tabs->addTab(tabGeometry    = new Panel(), "Geometry");
    auto&& vb = tabGeometry->vb();

    auto&& detDist    = new l_qt::spinReal(7, 2);
    auto&& detPixSize = new l_qt::spinReal(7, 3);
    auto&& detBeamX   = new l_qt::spinInt(7);
    auto&& detBeamY   = new l_qt::spinInt(7);

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
      gr.add({btn(hub.acts.get(hub.acts.IMG_ROTATE0)),     lbl("<rotate")});
      gr.add({btn(hub.acts.get(hub.acts.IMG_MIRROR_HORZ)), lbl("<mirror")});
    }

    auto&& cutLeft   = new l_qt::spinUint(3);
    auto&& cutRight  = new l_qt::spinUint(3);
    auto&& cutTop    = new l_qt::spinUint(3);
    auto&& cutBottom = new l_qt::spinUint(3);

    {
      auto&& gr = vb.gr();
      gr.add({btn(hub.acts.get(hub.acts.IMG_LINK_CUT)), lbl("cut"),
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

    auto&& polDegree = new l_qt::spinUint(1); polDegree->max(4);

    vb.hb().add(btn(hub.acts.get(hub.acts.BG_SELECT)))
           .add(btn(hub.acts.get(hub.acts.BG_SHOW)))
           .add(btn(hub.acts.get(hub.acts.BG_CLEAR)))
           .addStretch();
    vb.hb().add(lbl("Pol. degree")).add(polDegree)
           .addStretch();

    vb.addStretch();
  }

  {
    tabs->addTab(tabReflections = new Panel(), "Reflections");
    auto&& vb = tabReflections->vb();

    vb.hb().add(btn(hub.acts.get(hub.acts.REFL_SELECT)))
           .add(btn(hub.acts.get(hub.acts.BG_SHOW)))
           .add(btn(hub.acts.get(hub.acts.REFL_CLEAR)))
           .addStretch();

    box.addWidget((reflectionView_ = new ReflectionView(hub_)));

    hb = hbox();
    box.addLayout(hb);

    hb->addWidget((comboReflType_ = comboBox(calc::Reflection::typeStrLst())));
    hb->addStretch();
    hb->addWidget(iconButton(actions.addReflection));
    hb->addWidget(iconButton(actions.remReflection));

    auto vb = vbox();
    box.addLayout(vb);

    auto gb = gridLayout();
    vb->addLayout(gb);

    gb->addWidget(label("min"), 0, 0);
    gb->addWidget((spinRangeMin_ = spinDoubleCell(gui_cfg::em4_2, .0)), 0, 1);
    spinRangeMin_->setSingleStep(.1);
    gb->addWidget(label("max"), 0, 2);
    gb->addWidget((spinRangeMax_ = spinDoubleCell(gui_cfg::em4_2, .0)), 0, 3);
    spinRangeMax_->setSingleStep(.1);

    gb->addWidget(label("guess x"), 1, 0);
    gb->addWidget((spinGuessPeakX_ = spinDoubleCell(gui_cfg::em4_2, .0)), 1, 1);
    spinGuessPeakX_->setSingleStep(.1);
    gb->addWidget(label("y"), 1, 2);
    gb->addWidget((spinGuessPeakY_ = spinDoubleCell(gui_cfg::em4_2, .0)), 1, 3);
    spinGuessPeakY_->setSingleStep(.1);

    gb->addWidget(label("fwhm"), 2, 0);
    gb->addWidget((spinGuessFWHM_ = spinDoubleCell(gui_cfg::em4_2, .0)), 2, 1);
    spinGuessFWHM_->setSingleStep(.1);

    gb->addWidget(label("fit x"), 3, 0);
    gb->addWidget((readFitPeakX_ = readCell(gui_cfg::em4_2)), 3, 1);
    gb->addWidget(label("y"), 3, 2);
    gb->addWidget((readFitPeakY_ = readCell(gui_cfg::em4_2)), 3, 3);

    gb->addWidget(label("fwhm"), 4, 0);
    gb->addWidget((readFitFWHM_ = readCell(gui_cfg::em4_2)), 4, 1);

    gb->setColumnStretch(4, 1);


    vb.addStretch();
  }
}

//------------------------------------------------------------------------------
}
// eof
