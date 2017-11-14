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

#pragma once
#include <lib/qt/defs.hpp>
#include <lib/qt/win/layout.hpp>
#include <lib/qt/wgt_inc.hpp>
#include <lib/qt/win/tabs.hpp>
#include "../thehub.hpp"
#include <QFrame>

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------

dcl_sub_(GroupReflection, l_qt::group)
  GroupReflection();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(GroupGammaSlices, l_qt::group)
  GroupGammaSlices();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(GroupGammaRange, l_qt::group)
  GroupGammaRange();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(GroupPoints, l_qt::group)
  GroupPoints();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(GroupInterpolation, l_qt::group)
  GroupInterpolation();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(GroupDiagram, l_qt::group)
  GroupDiagram();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(TabPoints, l_qt::panel)
  dcl_sub_(ShowCols, l_qt::panel)
    ShowCols();
  dcl_end

  TabPoints();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(TabSave, l_qt::panel)
  TabSave();
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Frame, QFrame)
  atr_(core::data::CombinedSets::shr, sets);
  atr_(core::calc::FitParams::shr,    fp);

  atr_(l_qt::box*,  groups);
  atr_(l_qt::tabs*, tabs);
  atr_(l_qt::box*,  btns);

  Frame(strc title, QWidget*, Hub::rc);
dcl_end

//------------------------------------------------------------------------------
}}
// eof DOCS
