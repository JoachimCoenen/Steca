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

#include "dlg_polefigure.hpp"
#include <lib/qt/defs.inc>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------
using namespace l_qt::make_widgets;

dcl_sub_(TabGraph, l_qt::panel)
  TabGraph();
dcl_end

TabGraph::TabGraph() {}

//------------------------------------------------------------------------------

dcl_sub_(TabPointsSave, TabSave)
  TabPointsSave();
dcl_end

TabPointsSave::TabPointsSave() {}

//------------------------------------------------------------------------------

PoleFigure::PoleFigure(QWidget* parent, Hub::rc hub)
: base("Pole Figure", parent, hub)
{
  groups->add(new GroupReflection)
         .add(new GroupGammaSlices)
         .add(new GroupGammaRange)
         .add(new GroupPoints)
         .add(new GroupInterpolation);

  tabs->addTab(new TabPoints,     "Points")
       .addTab(new TabGraph,      "Graph")
       .addTab(new TabPointsSave, "Save");

  auto&& calcBtn = pushbtn("Calculate");
  auto&& ipolBtn = pushbtn("Interpolate");

  btns->add(calcBtn).add(ipolBtn);
}

//------------------------------------------------------------------------------
}}
// eof DOCS
