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

#ifndef CORE_FIT_FUN_H
#define CORE_FIT_FUN_H

#include "../typ/fun.hpp"
#include "../typ/curve.hpp"

namespace core {

namespace key {
extern c::str const
  PEAK, FWHM,
  POLYNOM, RAW, GAUSSIAN, LORENTZIAN, PSEUDOVOIGT1, PSEUDOVOIGT2;
}

namespace fit {
//------------------------------------------------------------------------------

void initFry();

//------------------------------------------------------------------------------
// a polynom(ial)

dcl_struct_sub_(Polynom, SimpleFun)
  Polynom(uint degree = 0);

  set_(degree, (uint))
  mth_(uint, degree, ())

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(real, avgY, (Range::rc xx, real const* parVals = nullptr))

  voi_(fit, (Curve::rc, Ranges::rc xx))
  fry_(Polynom, fromFit, (uint degree, Curve::rc, Ranges::rc xx))
dcl_end

//------------------------------------------------------------------------------
// Abstract peak function

enum class ePeakType {
  RAW, GAUSSIAN, LORENTZIAN, PSEUDOVOIGT1, PSEUDOVOIGT2,
  NUM_TYPES
};

dcl_struct_sub_(PeakFun, SimpleFun)
  fry_(c::give_me<PeakFun>, factory, (ePeakType))

  PeakFun();

  mth_(c::give_me<PeakFun>, clone, ())
  abs_mth_(ePeakType, type, ())

  atr_(Range,  range)
  atr_(peak_t, guessedPeak)
  atr_(fwhm_t, guessedFWHM)

  abs_mth_(peak_t, fittedPeak, ())
  abs_mth_(fwhm_t, fittedFWHM, ())
  abs_mth_(peak_t, peakError,  ())
  abs_mth_(fwhm_t, fwhmError,  ())

  voi_(reset, ())

  voi_inl_(fit, (Curve::rc curve), { fit(curve, range); })
  vir_voi_(fit, (Curve::rc, Range::rc))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(Raw, PeakFun)
  Raw();

  val_(ePeakType, type, (), ePeakType::RAW)

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(peak_t, fittedPeak, ())
  mth_(fwhm_t, fittedFWHM, ())
  mth_(peak_t, peakError,  ())
  mth_(fwhm_t, fwhmError,  ())

  voi_(fit, (Curve::rc, Range::rc))

private_
//  typ::Curve fittedCurve_;  // saved from fitting
//  void prepareY();

//  mutable uint  x_count_;
//  mutable real dx_;
//  mutable real sum_y_;
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(Gaussian, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA };

  Gaussian(real ampl = 1, real xShift = 0, real sigma = 1);

  val_(ePeakType, type, (), ePeakType::GAUSSIAN)

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(peak_t, fittedPeak, ())
  mth_(fwhm_t, fittedFWHM, ())
  mth_(peak_t, peakError,  ())
  mth_(fwhm_t, fwhmError,  ())
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(Lorentzian, PeakFun)
  enum { parAMPL, parXSHIFT, parGAMMA };

  Lorentzian(real ampl = 1, real xShift = 0, real gamma = 1);

  val_(ePeakType, type, (), ePeakType::LORENTZIAN)

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(peak_t, fittedPeak, ())
  mth_(fwhm_t, fittedFWHM, ())
  mth_(peak_t, peakError,  ())
  mth_(fwhm_t, fwhmError,  ())
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(PseudoVoigt1, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMAGAMMA, parETA };

  PseudoVoigt1(real ampl = 1, real xShift = 0, real sigmaGamma = 1,
               real eta = 0.1);

  val_(ePeakType, type, (), ePeakType::PSEUDOVOIGT1)

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(peak_t, fittedPeak, ())
  mth_(fwhm_t, fittedFWHM, ())
  mth_(peak_t, peakError,  ())
  mth_(fwhm_t, fwhmError,  ())
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(PseudoVoigt2, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA, parGAMMA, parETA };

  PseudoVoigt2(real ampl = 1, real xShift = 0, real sigma = 1,
               real gamma = 1, real eta = 0.1);

  val_(ePeakType, type, (), ePeakType::PSEUDOVOIGT2)

  mth_(real,  y, (real x, real const* parVals = nullptr))
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  mth_(peak_t, fittedPeak, ())
  mth_(fwhm_t, fittedFWHM, ())
  mth_(peak_t, peakError,  ())
  mth_(fwhm_t, fwhmError,  ())
dcl_end

//------------------------------------------------------------------------------
}}
#endif // FIT_FUN_H
