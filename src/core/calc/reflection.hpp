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
#include <lib/dev/inc/ptr.hpp>
#include "../fit/fit_fun.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

dcl_(Reflection) SHARED // TODO rename -> Peak ?
  atr_(fit::PeakFun::sh, peakFun);

  Reflection(fit::ePeakType = fit::ePeakType::RAW);

  voi_mut_(setType,  (fit::ePeakType));
  voi_mut_(setRange, (Range::rc));
  voi_mut_(setGuess, (peak_t::rc = peak_t(), fwhm_t = fwhm_t()));

private:
  void setPeakFun(fit::ePeakType);
  void setPeakFun(l::give_me<fit::PeakFun>);

  //  static str_vec::rc typeStrLst();
//  static rcstr       typeTag(fit::ePeakType);


//  fit::ePeakType type() const;

//  fit::PeakFunction::rc peakFunction() const;  // REMOVE

//  typ::Range::rc range() const;


//  void fit(typ::Curve::rc);

//_private


//public:
//  typ::JsonObj saveJson() const;
//  void loadJson(typ::JsonObj::rc) THROWS;
dcl_end

//------------------------------------------------------------------------------
}}
// eof
