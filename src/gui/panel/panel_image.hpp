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
#include "panel.hpp"
#include <core/data/sets.hpp>
#include <core/calc/fit_params.hpp>

namespace gui {
//------------------------------------------------------------------------------
struct ImageWidget;

dcl_sub_(PanelImage, Panel)
  friend struct ImageWidget;
  explicit PanelImage(Hub&);

private:
  Panel *tabImage, *tabCorrection;
  ImageWidget *wgtImage, *wgtCorrection;
  core::data::CombinedSets::shr sets;
  core::data::CombinedSet::shp  set;
  core::calc::FitParams::shr    fp;
  core::Image::shp              corrImage;

  bool  imageStepScale = false, imageShowOver = false;

  void renderTabs();
dcl_end

//------------------------------------------------------------------------------
}
// eof
