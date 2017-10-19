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

#pragma once
#include <lib/dev/defs.hpp>
#include <lib/qt/layout.hpp>
#include "../thehub.hpp"
#include <QFrame>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------

dcl_sub_(PanelReflection, l_qt::panel)
  PanelReflection();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PanelGammaSlices, l_qt::panel)
  PanelGammaSlices();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PanelGammaRange, l_qt::panel)
  PanelGammaRange();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PanelPoints, l_qt::panel)
  PanelPoints();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PanelInterpolation, l_qt::panel)
  PanelInterpolation();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PanelDiagram, l_qt::panel)
  PanelDiagram();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Frame, QFrame)
  Frame(strc title, QWidget*, Hub::rc);

  atr_(core::data::CombinedSets::shr, sets);
  atr_(core::calc::FitParams::shr,    fp);
dcl_end

//------------------------------------------------------------------------------
}}
// eof
