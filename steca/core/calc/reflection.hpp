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

#include <dev_lib/defs.hpp>
#include <dev_lib/inc/ptr.hpp>
#include "../fit/fit_fun.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

dcl_(Reflection)
  atr_(l::scoped<fit::PeakFun const>, peakFun);

//  static str_vec::rc typeStrLst();
//  static rcstr       typeTag(fit::ePeakType);

//  Reflection(fit::ePeakType = fit::ePeakType::RAW);

//  fit::ePeakType type() const;
//  void setType(fit::ePeakType);

//  fit::PeakFunction::rc peakFunction() const;  // REMOVE

//  typ::Range::rc range() const;
//  void setRange(typ::Range::rc);

//  void invalidateGuesses();

//  void setGuessPeak(peak_t::rc peak) { peakFunction_->setGuessedPeak(peak); }
//  void setGuessFWHM(fwhm_t fwhm)     { peakFunction_->setGuessedFWHM(fwhm); }

//  void fit(typ::Curve::rc);

//_private
//  void setPeakFunction(fit::ePeakType);
//  void setPeakFunction(fit::PeakFunction*);


//public:
//  typ::JsonObj saveJson() const;
//  void loadJson(typ::JsonObj::rc) THROWS;
dcl_end

//typedef typ::vec<shp_Reflection> Reflections;

//------------------------------------------------------------------------------
}}
// eof
