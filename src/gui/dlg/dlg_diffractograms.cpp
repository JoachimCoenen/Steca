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

#include "dlg_diffractograms.hpp"
#include <lib/qt/inc/defs.inc>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------
using namespace l_qt::make_widgets;

dcl_sub_(TabDiffractogramSave, TabSave)
  TabDiffractogramSave();
dcl_end

TabDiffractogramSave::TabDiffractogramSave() {}

//------------------------------------------------------------------------------

Diffractograms::Diffractograms(QWidget* parent, Hub::rc hub)
: base("", parent, hub)
{
  groups->add(new GroupGammaSlices)
         .add(new GroupGammaRange);

  tabs->addTab(new TabDiffractogramSave, "Save");
}

//------------------------------------------------------------------------------
}}
// eof
