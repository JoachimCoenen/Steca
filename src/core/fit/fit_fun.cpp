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

#include "fit_fun.hpp"
#include <lib/dev/inc/defs.inc>
#include "fit_methods.hpp"

namespace core {

namespace fit {
//------------------------------------------------------------------------------

Polynom::Polynom(uint degree) {
  setDegree(degree);
}

Polynom::ref Polynom::setDegree(uint degree) {
  base::setParCount(degree + 1);
  RTHIS
}

uint Polynom::degree() const {
  uint parCount = base::parCount();
  ENSURE_(parCount > 0)
  return parCount - 1;
}

real Polynom::y(real x, real const* parVals) const {
  real val = 0, xPow = 1;
  for_i_(pars.size()) {
    val += parVal(i, parVals) * xPow;
    xPow *= x;
  }
  return val;
}

real Polynom::dy(real x, uint i, real const*) const {
  return l::pow(x, i);
}

// REVIEW
real Polynom::avgY(Range::rc rgeX, real const* parVals) const {
  EXPECT_(rgeX.isDef())

  real w = rgeX.width();
  if (w <= 0)
    return y(rgeX.min, parVals);

  real minY = 0, maxY = 0, minPow = 1, maxPow = 1;

  for_i_(pars.size()) {
    real facY = parVal(i, parVals) / (i + 1);
    minY += facY * (minPow *= rgeX.min);
    maxY += facY * (maxPow *= rgeX.max);
  }

  return (1 / w) * (maxY - minY);
}

void Polynom::fit(Curve::rc curve, Ranges::rc ranges) {
  LevenbergMarquardt().fit(*this, curve.intersect(ranges));
}

Polynom Polynom::fromFit(uint degree, Curve::rc curve, Ranges::rc ranges) {
  Polynom poly(degree);
  poly.fit(curve, ranges);
  return poly;
}

//------------------------------------------------------------------------------

l::own<PeakFun> PeakFun::factory(ePeakType type) {
  switch (type) {
  case ePeakType::RAW:
    return l::owned(new Raw());
  case ePeakType::GAUSSIAN:
    return l::owned(new Gaussian());
  case ePeakType::LORENTZIAN:
    return l::owned(new Lorentzian());
  case ePeakType::PSEUDOVOIGT1:
    return l::owned(new PseudoVoigt1());
  case ePeakType::PSEUDOVOIGT2:
    return l::owned(new PseudoVoigt2());
  default:
    NEVER return l::own<Self>(nullptr);
  }
}

l::own<PeakFun> PeakFun::clone() const {
  auto  f = factory(type());
  auto& r = mut(*f);

  // do not copy guessed values
  mut(r.pars)  = pars;
  mut(r.range) = range;

  return f;
}

PeakFun::PeakFun() : guessedPeak(), guessedFWHM(l::flt_nan) {}

PeakFun::ref PeakFun::reset() {
  base::reset();
  setGuessedPeak(peak_t(0, 0));
  setGuessedFWHM(fwhm_t(0));
  RTHIS
}

void PeakFun::fit(Curve::rc curve, Range::rc range) {
  Curve c = prepareFit(curve, range);
  if (c.isEmpty())
    return;

//  if (!guessedPeak().isValid()) {  // calculate guesses // TODO caching temporarily disabled, until it works correctly
    uint peakIndex  = c.maxYindex();
    auto peakTth    = c.xs.at(peakIndex);
    auto peakIntens = c.ys.at(peakIndex);

    // half-maximum indices
    uint hmi1 = peakIndex, hmi2 = peakIndex;

    // left
    for (uint i = peakIndex; i-- > 0;) {
      hmi1 = i;
      if (c.ys.at(i) < peakIntens / 2) break;
    }

    // right
    for (uint i = peakIndex, iEnd = c.size(); i < iEnd; ++i) {
      hmi2 = i;
      if (c.ys.at(i) < peakIntens / 2) break;
    }

    mut(guessedPeak).set(peakTth, peakIntens);
    mut(guessedFWHM) = fwhm_t(c.xs.at(hmi2) - c.xs.at(hmi1));
//  }

  LevenbergMarquardt().fit(*this, c);
}

Curve PeakFun::prepareFit(Curve::rc curve, Range::rc range) {
  reset();
  return curve.intersect(range);
}

//------------------------------------------------------------------------------

Raw::Raw() {
}

real Raw::y(real x, real const*) const {
  if (!x_count || !range.contains(x))
    return 0;

  uint i = l::to_u(l::bound(int64(0), l::floor((x - range.min) / dx), l::to_num<int64>(x_count) - 1));
  return fittedCurve.ys.at(i);
}

real Raw::dy(real, uint, real const*) const {
  return 0; // fake
}

peak_t Raw::fittedPeak() const {
  if (l::isnan(sum_y))
    sum_y = fittedCurve.sumY();

  return peak_t(range.center(),  // approximate x
                sum_y);
}

fwhm_t Raw::fittedFWHM() const {
  return fwhm_t(range.width());  // kind-of
}

peak_t Raw::peakError() const {
  return peak_t(0, 0);
}

fwhm_t Raw::fwhmError() const {
  return fwhm_t(0);
}

Raw::ref Raw::setRange(Range::rc r) {
  base::setRange(r);
  prepareY();
  RTHIS;
}

void Raw::fit(Curve::rc curve, Range::rc range) {
  fittedCurve = prepareFit(curve, range);  // do no more than this
  prepareY();
}

void Raw::prepareY() {
  if (range.isEmpty() || fittedCurve.isEmpty()) {
    x_count = 0;
    dx      = 0;
  } else {
    x_count = fittedCurve.size();
    dx      = range.width() / x_count;
  }

  sum_y = l::flt_nan;
}

//------------------------------------------------------------------------------

Gaussian::Gaussian(real ampl, real xShift, real sigma) {
  setParCount(3);

  auto& parAmpl   = pars.at(parAMPL);
  auto& parXShift = pars.at(parXSHIFT);
  auto& parSigma  = pars.at(parSIGMA);

  mut(parAmpl).set(ampl, 0);
  mut(parXShift).set(xShift, 0);
  mut(parSigma).set(sigma, 0);
}

Gaussian::ref Gaussian::setGuessedPeak(peak_t::rc xy) {
  base::setGuessedPeak(xy);
  setParVal(parXSHIFT, xy.x);
  setParVal(parAMPL,   xy.y);
  RTHIS
}

Gaussian::ref Gaussian::setGuessedFWHM(fwhm_t fwhm) {
  base::setGuessedFWHM(fwhm);
  // sigma = FWHM * 1/4 * (SQRT(2)/SQRT(ln(2))) = FWHM * 0.424661
  setParVal(parSIGMA, fwhm * 0.424661);
  RTHIS
}

real Gaussian::y(real x, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real sigma  = parVal(parSIGMA,  parVals);

  real arg = (x - xShift) / sigma;
  real exa = exp(-0.5 * arg * arg);

  return ampl * exa;
}

real Gaussian::dy(real x, uint parIndex, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real sigma  = parVal(parSIGMA,  parVals);

  real arg = (x - xShift) / sigma;
  real exa = exp(-0.5 * arg * arg);

  switch (parIndex) {
  case parAMPL:
    return exa;
  case parXSHIFT:
    return ampl * exa * (x - xShift) / (sigma * sigma);
  case parSIGMA:
    return ampl * exa * ((x - xShift) * (x - xShift)) / (sigma * sigma * sigma);
  default:
    NEVER return 0;
  }
}

peak_t Gaussian::fittedPeak() const {
  return peak_t(parVal(parXSHIFT), parVal(parAMPL));
}

fwhm_t Gaussian::fittedFWHM() const {
  return fwhm_t(parVal(parSIGMA) / 0.424661);
}

peak_t Gaussian::peakError() const {
  return peak_t(parErr(parXSHIFT), parErr(parAMPL));
}

fwhm_t Gaussian::fwhmError() const {
  // REVIEW
  return fwhm_t(parErr(parSIGMA));
}

//------------------------------------------------------------------------------

Lorentzian::Lorentzian(real ampl, real xShift, real gamma) {
  setParCount(3);

  auto& parAmpl   = pars.at(parAMPL);
  auto& parXShift = pars.at(parXSHIFT);
  auto& parGamma  = pars.at(parGAMMA);

  mut(parAmpl).set(ampl, 0);
  mut(parXShift).set(xShift, 0);
  mut(parGamma).set(gamma, 0);
}

Lorentzian::ref Lorentzian::setGuessedPeak(peak_t::rc xy) {
  base::setGuessedPeak(xy);
  setParVal(parXSHIFT, xy.x);
  setParVal(parAMPL,   xy.y);
  RTHIS
}

Lorentzian::ref Lorentzian::setGuessedFWHM(fwhm_t fwhm) {
  base::setGuessedFWHM(fwhm);
  // gamma = HWHM = FWHM / 2
  setParVal(parGAMMA, fwhm / 2);
  RTHIS
}

real Lorentzian::y(real x, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real gamma  = parVal(parGAMMA,  parVals);

  real arg = (x - xShift) / gamma;
  return ampl / (1 + arg * arg);
}

real Lorentzian::dy(real x, uint parIndex, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real gamma  = parVal(parGAMMA,  parVals);

  real arg1 = (x - xShift) / gamma;
  real arg2 = arg1 * arg1;
  real arg3 = (1 + arg2) * (1 + arg2);

  switch (parIndex) {
  case parAMPL:
    return 1 / (1 + arg2);
  case parXSHIFT:
    return 2 * ampl * (x - xShift) / (arg3 * gamma * gamma);
  case parGAMMA:
    return 2 * ampl * (x - xShift) * (x - xShift) /
           (arg3 * gamma * gamma * gamma);
  default:
    NEVER return 0;
  }
}

peak_t Lorentzian::fittedPeak() const {
  return peak_t(parVal(parXSHIFT), parVal(parAMPL));
}

fwhm_t Lorentzian::fittedFWHM() const {
  return fwhm_t(parVal(parGAMMA) * 2);
}

peak_t Lorentzian::peakError() const {
  return peak_t(parErr(parXSHIFT), parErr(parAMPL));
}

fwhm_t Lorentzian::fwhmError() const {
  return fwhm_t(parErr(parGAMMA));
}

//------------------------------------------------------------------------------

PseudoVoigt1::PseudoVoigt1(real ampl, real xShift, real sigmaGamma,
                           real eta) {
  setParCount(4);

  auto& parAmpl       = pars.at(parAMPL);
  auto& parXShift     = pars.at(parXSHIFT);
  auto& parSigmaGamma = pars.at(parSIGMAGAMMA);
  auto& parEta        = pars.at(parETA);

  mut(parAmpl).set(ampl, 0);
  mut(parXShift).set(xShift, 0);
  mut(parSigmaGamma).set(sigmaGamma, 0);
  mut(parEta).set(eta, 0);
}

PseudoVoigt1::ref PseudoVoigt1::setGuessedPeak(peak_t::rc xy) {
  base::setGuessedPeak(xy);
  setParVal(parXSHIFT, xy.x);
  setParVal(parAMPL,   xy.y);
  RTHIS
}

PseudoVoigt1::ref PseudoVoigt1::setGuessedFWHM(fwhm_t fwhm) {
  base::setGuessedFWHM(fwhm);
  setParVal(parSIGMAGAMMA, fwhm / 2);
  RTHIS
}

real PseudoVoigt1::y(real x, real const* parVals) const {
  real ampl       = parVal(parAMPL,       parVals);
  real xShift     = parVal(parXSHIFT,     parVals);
  real sigmaGamma = parVal(parSIGMAGAMMA, parVals);
  real eta        = parVal(parETA,        parVals);

  real arg      = (x - xShift) / sigmaGamma;
  real arg2     = arg * arg;
  real gaussian = ampl * exp(-arg2 * log(2.0));
  real lorentz  = ampl / (1 + arg2);

  return (1 - eta) * gaussian + eta * lorentz;
}

real PseudoVoigt1::dy(real x, uint parIndex, real const* parVals) const {
  real ampl       = parVal(parAMPL,       parVals);
  real xShift     = parVal(parXSHIFT,     parVals);
  real sigmaGamma = parVal(parSIGMAGAMMA, parVals);
  real eta        = parVal(parETA,        parVals);

  real arg1 = (x - xShift) / sigmaGamma;
  real arg2 = arg1 * arg1;
  real arg3 = exp(-arg2 * log(2.0));
  real arg4 = 1 + arg2;

  switch (parIndex) {
  case parAMPL:
    return eta / arg4 + (1 - eta) * arg3;
  case parXSHIFT:
    return eta * 2 * ampl * (x - xShift) /
               (arg4 * arg4 * sigmaGamma * sigmaGamma) +
           (1 - eta) * 2 * ampl * (x - xShift) * log(2.0) * arg3 /
               (sigmaGamma * sigmaGamma);
  case parSIGMAGAMMA:
    return eta * 2 * ampl * (x - xShift) * (x - xShift) /
               (arg4 * arg4 * sigmaGamma * sigmaGamma * sigmaGamma) +
           (1 - eta) * 2 * ampl * (x - xShift) * (x - xShift) * log(2.0) *
               arg3 / (sigmaGamma * sigmaGamma * sigmaGamma);
  case parETA:
    return ampl / arg4 - ampl * arg3;
  default:
    NEVER return 0;
  }
}

peak_t PseudoVoigt1::fittedPeak() const {
  return peak_t(parVal(parXSHIFT), parVal(parAMPL));
}

fwhm_t PseudoVoigt1::fittedFWHM() const {
  return fwhm_t(parVal(parSIGMAGAMMA) * 2);
}

peak_t PseudoVoigt1::peakError() const {
  return peak_t(parErr(parXSHIFT), parErr(parAMPL));
}

fwhm_t PseudoVoigt1::fwhmError() const {
  return fwhm_t(parErr(parSIGMAGAMMA));
}

//------------------------------------------------------------------------------

PseudoVoigt2::PseudoVoigt2(real ampl, real mu, real hwhmG, real hwhmL,
                           real eta) {
  setParCount(5);

  auto& parAmpl  = pars.at(parAMPL);
  auto& parMu    = pars.at(parXSHIFT);
  auto& parHwhmG = pars.at(parSIGMA);
  auto& parHwhmL = pars.at(parGAMMA);
  auto& parEta   = pars.at(parETA);

  mut(parAmpl).set(ampl, 0);
  mut(parMu).set(mu, 0);
  mut(parHwhmG).set(hwhmG, 0);
  mut(parHwhmL).set(hwhmL, 0);
  mut(parEta).set(eta, 0);
}

PseudoVoigt2::ref PseudoVoigt2::setGuessedPeak(peak_t::rc xy) {
  base::setGuessedPeak(xy);
  setParVal(parXSHIFT, xy.x);
  setParVal(parAMPL,   xy.y);
  RTHIS
}

PseudoVoigt2::ref PseudoVoigt2::setGuessedFWHM(fwhm_t fwhm) {
  base::setGuessedFWHM(fwhm);
  setParVal(parSIGMA, fwhm * 0.424661);
  setParVal(parGAMMA, fwhm / 2);
  RTHIS
}

real PseudoVoigt2::y(real x, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real sigma  = parVal(parSIGMA,  parVals);
  real gamma  = parVal(parGAMMA,  parVals);
  real eta    = parVal(parETA,    parVals);

  real argG     = (x - xShift) / sigma;
  real argG2    = argG * argG;
  real gaussian = ampl * exp(-argG2 * log(2.0));

  real argL    = (x - xShift) / gamma;
  real argL2   = argL * argL;
  real lorentz = ampl / (1 + argL2);

  return (1 - eta) * gaussian + eta * lorentz;
}

real PseudoVoigt2::dy(real x, uint parIndex, real const* parVals) const {
  real ampl   = parVal(parAMPL,   parVals);
  real xShift = parVal(parXSHIFT, parVals);
  real sigma  = parVal(parSIGMA,  parVals);
  real gamma  = parVal(parGAMMA,  parVals);
  real eta    = parVal(parETA,    parVals);

  real argG1 = (x - xShift) / sigma;
  real argG2 = argG1 * argG1;
  real argG3 = exp(-argG2 * log(2.0));

  real argL1 = (x - xShift) / gamma;
  real argL2 = argL1 * argL1;
  real argL3 = 1 + argL2;

  switch (parIndex) {
  case parAMPL:
    return eta / argL3 + (1 - eta) * argG3;
  case parXSHIFT:
    return eta * 2 * ampl * (x - xShift) / (argL3 * argL3 * gamma * gamma) +
           (1 - eta) * 2 * ampl * (x - xShift) * log(2.0) * argG3 /
               (sigma * sigma);
  case parSIGMA:
    return (1 - eta) * 2 * ampl * (x - xShift) * (x - xShift) * log(2.0) *
           argG3 / (sigma * sigma * sigma);
  case parGAMMA:
    return eta * 2 * ampl * (x - xShift) * (x - xShift) /
           (argL3 * argL3 * gamma * gamma * gamma);
  case parETA:
    return ampl / argL3 - ampl * argG3;
  default:
    NEVER return 0;
  }
}

peak_t PseudoVoigt2::fittedPeak() const {
  return peak_t(parVal(parXSHIFT), parVal(parAMPL));
}

fwhm_t PseudoVoigt2::fittedFWHM() const {
  real eta = parVal(parETA);
  return fwhm_t(((1 - eta) * parVal(parSIGMA) / 0.424661 +
                 eta * parVal(parGAMMA) * 2) / 2);
}

peak_t PseudoVoigt2::peakError() const {
  return peak_t(parErr(parXSHIFT), parErr(parAMPL));
}

fwhm_t PseudoVoigt2::fwhmError() const {
  // REVIEW
  return fwhm_t(parErr(parSIGMA) +
                parErr(parGAMMA));
}

//------------------------------------------------------------------------------
}}
// eof
