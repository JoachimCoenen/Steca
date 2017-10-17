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
#include "../typ/curve.hpp"
#include "../typ/fun.hpp"
#include "../typ/range.hpp"
#include "../types.hpp"

namespace core { namespace fit {
//------------------------------------------------------------------------------

void initFry();

//------------------------------------------------------------------------------
// a polynom(ial)

dcl_sub_(Polynom, SimpleFun)
  Polynom(uint degree = 0);

  set_(setDegree, (uint));
  mth_(uint, degree, ());

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(real, avgY, (Range::rc xx, real const* parVals = nullptr));

  mut_(fit, (Curve::rc, Ranges::rc xx));
  fry_(Polynom, fromFit, (uint degree, Curve::rc, Ranges::rc xx));
dcl_end

//------------------------------------------------------------------------------
// Abstract peak function

dcl_sub_(PeakFun, SimpleFun) SHARED
  enum class eType {
    RAW, GAUSSIAN, LORENTZIAN, PSEUDOVOIGT1, PSEUDOVOIGT2,
    NUM_TYPES
  };

  fry_(l::own<PeakFun>, factory, (eType));

  PeakFun();

  mth_(l::own<PeakFun>, clone, ());
  virtual mth_(eType, type, ()) = 0;
  mth_(strc, sType, ());

  atr_(Range,  range);
  atr_(peak_t, guessedPeak);
  atr_(fwhm_t, guessedFWHM);

  virtual mth_(peak_t, fittedPeak, ()) = 0;
  virtual mth_(fwhm_t, fittedFWHM, ()) = 0;
  virtual mth_(peak_t, peakError,  ()) = 0;
  virtual mth_(fwhm_t, fwhmError,  ()) = 0;

  mut_(reset, ());

  virtual mut_(setRange, (Range::rc));
  virtual mut_(setGuess, (peak_t::rc, fwhm_t));

  mut_(fit, (Curve::rc curve)) { fit(curve, range); }
  virtual mut_(fit, (Curve::rc, Range::rc));

  protected:
  mth_mut_(Curve, prepareFit, (Curve::rc curve, Range::rc range));
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Raw, PeakFun)
  Raw();

  mth_(eType, type, ()) RET_(eType::RAW)

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(peak_t, fittedPeak, ());
  mth_(fwhm_t, fittedFWHM, ());
  mth_(peak_t, peakError,  ());
  mth_(fwhm_t, fwhmError,  ());

  mut_(setRange, (Range::rc r));

  mut_(fit, (Curve::rc, Range::rc));

private:
  Curve fittedCurve;  // saved from fitting
  void prepareY();

  mutable uint x_count;
  mutable real dx;
  mutable real sum_y;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Gaussian, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA };

  Gaussian(real ampl = 1, real xShift = 0, real sigma = 1);

  mth_(eType, type, ()) RET_(eType::GAUSSIAN)

  mut_(setGuess, (peak_t::rc, fwhm_t));

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(peak_t, fittedPeak, ());
  mth_(fwhm_t, fittedFWHM, ());
  mth_(peak_t, peakError,  ());
  mth_(fwhm_t, fwhmError,  ());
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Lorentzian, PeakFun)
  enum { parAMPL, parXSHIFT, parGAMMA };

  Lorentzian(real ampl = 1, real xShift = 0, real gamma = 1);

  mth_(eType, type, ()) RET_(eType::LORENTZIAN)

  mut_(setGuess, (peak_t::rc, fwhm_t));

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(peak_t, fittedPeak, ());
  mth_(fwhm_t, fittedFWHM, ());
  mth_(peak_t, peakError,  ());
  mth_(fwhm_t, fwhmError,  ());
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PseudoVoigt1, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMAGAMMA, parETA };

  PseudoVoigt1(real ampl = 1, real xShift = 0, real sigmaGamma = 1,
               real eta = 0.1);

  mth_(eType, type, ()) RET_(eType::PSEUDOVOIGT1)

  mut_(setGuess, (peak_t::rc, fwhm_t));

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(peak_t, fittedPeak, ());
  mth_(fwhm_t, fittedFWHM, ());
  mth_(peak_t, peakError,  ());
  mth_(fwhm_t, fwhmError,  ());
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(PseudoVoigt2, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA, parGAMMA, parETA };

  PseudoVoigt2(real ampl = 1, real xShift = 0, real sigma = 1,
               real gamma = 1, real eta = 0.1);

  mth_(eType, type, ()) RET_(eType::PSEUDOVOIGT2)

  mut_(setGuess, (peak_t::rc, fwhm_t));

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  mth_(peak_t, fittedPeak, ());
  mth_(fwhm_t, fittedFWHM, ());
  mth_(peak_t, peakError,  ());
  mth_(fwhm_t, fwhmError,  ());
dcl_end

//------------------------------------------------------------------------------
}}
