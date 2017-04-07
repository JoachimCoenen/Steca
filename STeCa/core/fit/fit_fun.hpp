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

dcl_struct_sub (Polynom, SimpleFun)
  Polynom(uint degree = 0);

  _set (degree, (uint))
  _mth (uint, degree, ())

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (real, avgY, (Range::rc xx, real const* parVals = nullptr))

  _voi (fit, (Curve::rc, Ranges::rc xx))
  _fry (Polynom, fromFit, (uint degree, Curve::rc, Ranges::rc xx))
dcl_struct_end

//------------------------------------------------------------------------------
// Abstract peak function

enum class ePeakType {
  RAW, GAUSSIAN, LORENTZIAN, PSEUDOVOIGT1, PSEUDOVOIGT2,
  NUM_TYPES
};

dcl_struct_sub (PeakFun, SimpleFun)
  _fry (c::give_me<PeakFun>, factory, (ePeakType))

  PeakFun();

  _mth (c::give_me<PeakFun>, clone, ())
  _abs_mth (ePeakType, type, ())

  _atr (Range,  range)
  _atr (peak_t, guessedPeak)
  _atr (fwhm_t, guessedFWHM)

  _abs_mth (peak_t, fittedPeak, ())
  _abs_mth (fwhm_t, fittedFWHM, ())
  _abs_mth (peak_t, peakError,  ())
  _abs_mth (fwhm_t, fwhmError,  ())

  _voi (reset, ())

  _voi_inl (fit, (Curve::rc curve), { fit(curve, range); })
  _vir_voi (fit, (Curve::rc, Range::rc))
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub (Raw, PeakFun)
  Raw();

  _val (ePeakType, type, (), ePeakType::RAW)

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (peak_t, fittedPeak, ())
  _mth (fwhm_t, fittedFWHM, ())
  _mth (peak_t, peakError,  ())
  _mth (fwhm_t, fwhmError,  ())

  _voi (fit, (Curve::rc, Range::rc))

_private
//  typ::Curve fittedCurve_;  // saved from fitting
//  void prepareY();

//  mutable uint  x_count_;
//  mutable real dx_;
//  mutable real sum_y_;
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub (Gaussian, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA };

  Gaussian(real ampl = 1, real xShift = 0, real sigma = 1);

  _val (ePeakType, type, (), ePeakType::GAUSSIAN)

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (peak_t, fittedPeak, ())
  _mth (fwhm_t, fittedFWHM, ())
  _mth (peak_t, peakError,  ())
  _mth (fwhm_t, fwhmError,  ())
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub (Lorentzian, PeakFun)
  enum { parAMPL, parXSHIFT, parGAMMA };

  Lorentzian(real ampl = 1, real xShift = 0, real gamma = 1);

  _val (ePeakType, type, (), ePeakType::LORENTZIAN)

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (peak_t, fittedPeak, ())
  _mth (fwhm_t, fittedFWHM, ())
  _mth (peak_t, peakError,  ())
  _mth (fwhm_t, fwhmError,  ())
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub (PseudoVoigt1, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMAGAMMA, parETA };

  PseudoVoigt1(real ampl = 1, real xShift = 0, real sigmaGamma = 1,
               real eta = 0.1);

  _val (ePeakType, type, (), ePeakType::PSEUDOVOIGT1)

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (peak_t, fittedPeak, ())
  _mth (fwhm_t, fittedFWHM, ())
  _mth (peak_t, peakError,  ())
  _mth (fwhm_t, fwhmError,  ())
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub (PseudoVoigt2, PeakFun)
  enum { parAMPL, parXSHIFT, parSIGMA, parGAMMA, parETA };

  PseudoVoigt2(real ampl = 1, real xShift = 0, real sigma = 1,
               real gamma = 1, real eta = 0.1);

  _val (ePeakType, type, (), ePeakType::PSEUDOVOIGT2)

  _mth (real,  y, (real x, real const* parVals = nullptr))
  _mth (real, dy, (real x, uint parIdx, real const* parVals = nullptr))

  _mth (peak_t, fittedPeak, ())
  _mth (fwhm_t, fittedFWHM, ())
  _mth (peak_t, peakError,  ())
  _mth (fwhm_t, fwhmError,  ())
dcl_struct_end

//------------------------------------------------------------------------------
}}
#endif // FIT_FUN_H
