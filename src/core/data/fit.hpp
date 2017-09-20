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
#include <lib/dev/inc/ptr.hpp>
#include "../calc/reflection.hpp"
#include "../typ/def.hpp"
#include "../typ/range.hpp"

namespace core { namespace data {
//------------------------------------------------------------------------------

dcl_(Fit) SHARED
  using Reflection = calc::Reflection;

  enum eWhat { NONE, BACKGROUND, PEAK };

  atr_(Ranges, bg);
  atr_(l::vec<Reflection::sh>, refls);
  atr_(Reflection::shp, currRefl);
dcl_end

//------------------------------------------------------------------------------
}}
