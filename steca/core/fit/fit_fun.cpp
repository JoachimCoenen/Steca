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
#include <dev_lib/defs.inc>
#include "fit_methods.hpp"

namespace core {

namespace fit {
//------------------------------------------------------------------------------

void initFry() {
  ONLY_ONCE

  Fun::initFry();

  Fun::addMaker(key::POLYNOM,      l::owned(new Fun::fryFun::maker<Gaussian>));
  Fun::addMaker(key::RAW,          l::owned(new Fun::fryFun::maker<Raw>));
  Fun::addMaker(key::GAUSSIAN,     l::owned(new Fun::fryFun::maker<Gaussian>));
  Fun::addMaker(key::LORENTZIAN,   l::owned(new Fun::fryFun::maker<Lorentzian>));
  Fun::addMaker(key::PSEUDOVOIGT1, l::owned(new Fun::fryFun::maker<PseudoVoigt1>));
  Fun::addMaker(key::PSEUDOVOIGT2, l::owned(new Fun::fryFun::maker<PseudoVoigt2>));
}

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

Json Polynom::save() const {
  return base::save()
      .add(key::TYPE, key::POLYNOM);
}

Polynom::ref Polynom::load(Json::rc json) may_err {
  base::load(json);
  RTHIS
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
  mut(guessedPeak).set(0, 0);
  mut(guessedFWHM) = 0;
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
    mut(guessedFWHM) = c.xs.at(hmi2) - c.xs.at(hmi1);
//  }

  LevenbergMarquardt().fit(*this, c);
}

Json PeakFun::save() const {
  return base::save()
      .add(key::RANGE, range)
      .add(key::PEAK, guessedPeak.save())
      .add(key::FWHM, guessedFWHM);
}

void PeakFun::loadJson(JsonObj::rc obj) THROWS {
  super::loadJson(obj);
  range_ = obj.loadRange(json_key::RANGE);
  guessedPeak_.loadJson(obj.loadObj(json_key::PEAK));
  guessedFWHM_ = obj.loadQreal(json_key::FWHM);
}

Curve PeakFun::prepareFit(Curve::rc curve, Range::rc range) {
  reset();
  return curve.intersect(range);
}

//------------------------------------------------------------------------------

Raw::Raw() {
}

qreal Raw::y(qreal x, qreal const* / *parValues* /) const {
  if (!x_count_ || !range_.contains(x))
    return 0;

  uint i = to_u(l::bound(0, qFloor((x - range_.min) / dx_), to_i(x_count_) - 1));
  return fittedCurve_.y(i);
}

qreal Raw::dy(qreal, uint, qreal const*) const {
  return 0; // fake
}

peak_t Raw::fittedPeak() const {
  if (l::isnan(sum_y_))
    sum_y_ = fittedCurve_.sumY();

  return peak_t(range_.center(),  // approximate x
                sum_y_);
}

fwhm_t Raw::fittedFWHM() const {
  return fwhm_t(range_.width());  // kind-of
}

peak_t Raw::peakError() const {
  return peak_t(0, 0);
}

fwhm_t Raw::fwhmError() const {
  return 0;
}

void Raw::setRange(Range::rc range) {
  super::setRange(range);
  prepareY();
}

void Raw::fit(Curve::rc curve, Range::rc range) {
  fittedCurve_ = prepareFit(curve, range);  // do no more than this
  prepareY();
}

void Raw::prepareY() {
  if (range_.isEmpty() || fittedCurve_.isEmpty()) {
    x_count_ = 0;
    dx_      = 0;
  } else {
    x_count_ = fittedCurve_.count();
    dx_      = range_.width() / x_count_;
  }

  sum_y_ = NAN;
}

JsonObj Raw::saveJson() const {
  return super::saveJson().saveString(json_key::TYPE, json_fun_key::RAW);
}

//------------------------------------------------------------------------------

Gaussian::Gaussian(qreal ampl, qreal xShift, qreal sigma) {
  setParameterCount(3);

  auto& parAmpl   = parameters_[parAMPL];
  auto& parXShift = parameters_[parXSHIFT];
  auto& parSigma  = parameters_[parSIGMA];

  parAmpl.setValueRange(0, flt_inf);
  parAmpl.setValue(ampl, 0);

  parXShift.setValue(xShift, 0);

  parSigma.setValueRange(0, flt_inf);
  parSigma.setValue(sigma, 0);
}

qreal Gaussian::y(qreal x, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal sigma  = parValue(parSIGMA, parValues);

  qreal arg = (x - xShift) / sigma;
  qreal exa = exp(-0.5 * arg * arg);

  return ampl * exa;
}

qreal Gaussian::dy(qreal x, uint parIndex, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal sigma  = parValue(parSIGMA, parValues);

  qreal arg = (x - xShift) / sigma;
  qreal exa = exp(-0.5 * arg * arg);

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

void Gaussian::setGuessedPeak(peak_t::rc xy) {
  super::setGuessedPeak(xy);
  setValue(parXSHIFT, xy.x);
  setValue(parAMPL, xy.y);
}

void Gaussian::setGuessedFWHM(fwhm_t fwhm) {
  super::setGuessedFWHM(fwhm);
  // sigma = FWHM * 1/4 * (SQRT(2)/SQRT(ln(2))) = FWHM * 0.424661
  setValue(parSIGMA, fwhm * 0.424661);
}

peak_t Gaussian::fittedPeak() const {
  return peak_t(parameters_.at(parXSHIFT).value(), parameters_.at(parAMPL).value());
}

fwhm_t Gaussian::fittedFWHM() const {
  return fwhm_t(parameters_.at(parSIGMA).value() / 0.424661);
}

peak_t Gaussian::peakError() const {
  return peak_t(parameters_.at(parXSHIFT).error(), parameters_.at(parAMPL).error());
}

fwhm_t Gaussian::fwhmError() const {
  // REVIEW
  return fwhm_t(parameters_.at(parSIGMA).error());
}

JsonObj Gaussian::saveJson() const {
  return super::saveJson().saveString(json_key::TYPE, json_fun_key::GAUSSIAN);
}

//------------------------------------------------------------------------------

Lorentzian::Lorentzian(qreal ampl, qreal xShift, qreal gamma) {
  setParameterCount(3);

  auto& parAmpl   = parameters_[parAMPL];
  auto& parXShift = parameters_[parXSHIFT];
  auto& parGamma  = parameters_[parGAMMA];

  parAmpl.setValueRange(0, flt_inf);
  parAmpl.setValue(ampl, 0);

  parXShift.setValue(xShift, 0);

  parGamma.setValueRange(0, flt_inf);
  parGamma.setValue(gamma, 0);
}

qreal Lorentzian::y(qreal x, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal gamma  = parValue(parGAMMA, parValues);

  qreal arg = (x - xShift) / gamma;
  return ampl / (1 + arg * arg);
}

qreal Lorentzian::dy(qreal x, uint parIndex, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal gamma  = parValue(parGAMMA, parValues);

  qreal arg1 = (x - xShift) / gamma;
  qreal arg2 = arg1 * arg1;
  qreal arg3 = (1 + arg2) * (1 + arg2);

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

void Lorentzian::setGuessedPeak(peak_t::rc xy) {
  super::setGuessedPeak(xy);
  setValue(parXSHIFT, xy.x);
  setValue(parAMPL, xy.y);
}

void Lorentzian::setGuessedFWHM(fwhm_t fwhm) {
  super::setGuessedFWHM(fwhm);
  // gamma = HWHM = FWHM / 2
  setValue(parGAMMA, fwhm / 2);
}

peak_t Lorentzian::fittedPeak() const {
  return peak_t(parameters_.at(parXSHIFT).value(), parameters_.at(parAMPL).value());
}

fwhm_t Lorentzian::fittedFWHM() const {
  return fwhm_t(parameters_.at(parGAMMA).value() * 2);
}

peak_t Lorentzian::peakError() const {
  return peak_t(parameters_.at(parXSHIFT).error(), parameters_.at(parAMPL).error());
}

fwhm_t Lorentzian::fwhmError() const {
  return fwhm_t(parameters_.at(parGAMMA).error());
}

JsonObj Lorentzian::saveJson() const {
  return super::saveJson().saveString(json_key::TYPE, json_fun_key::LORENTZIAN);
}

//------------------------------------------------------------------------------

PseudoVoigt1::PseudoVoigt1(qreal ampl, qreal xShift, qreal sigmaGamma,
                           qreal eta) {
  setParameterCount(4);

  auto& parAmpl       = parameters_[parAMPL];
  auto& parXShift     = parameters_[parXSHIFT];
  auto& parSigmaGamma = parameters_[parSIGMAGAMMA];
  auto& parEta        = parameters_[parETA];

  parAmpl.setValueRange(0, flt_inf);
  parAmpl.setValue(ampl, 0);

  parXShift.setValue(xShift, 0);

  parSigmaGamma.setValueRange(0, flt_inf);
  parSigmaGamma.setValue(sigmaGamma, 0);

  parEta.setValueRange(0, 1);
  parEta.setValue(eta, 0);
}

qreal PseudoVoigt1::y(qreal x, qreal const* parValues) const {
  qreal ampl       = parValue(parAMPL, parValues);
  qreal xShift     = parValue(parXSHIFT, parValues);
  qreal sigmaGamma = parValue(parSIGMAGAMMA, parValues);
  qreal eta        = parValue(parETA, parValues);

  qreal arg      = (x - xShift) / sigmaGamma;
  qreal arg2     = arg * arg;
  qreal gaussian = ampl * exp(-arg2 * log(2.0));
  qreal lorentz  = ampl / (1 + arg2);

  return (1 - eta) * gaussian + eta * lorentz;
}

qreal PseudoVoigt1::dy(qreal x, uint parIndex, qreal const* parValues) const {
  qreal ampl       = parValue(parAMPL, parValues);
  qreal xShift     = parValue(parXSHIFT, parValues);
  qreal sigmaGamma = parValue(parSIGMAGAMMA, parValues);
  qreal eta        = parValue(parETA, parValues);

  qreal arg1 = (x - xShift) / sigmaGamma;
  qreal arg2 = arg1 * arg1;
  qreal arg3 = exp(-arg2 * log(2.0));
  qreal arg4 = 1 + arg2;

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

void PseudoVoigt1::setGuessedPeak(peak_t::rc xy) {
  super::setGuessedPeak(xy);
  setValue(parXSHIFT, xy.x);
  setValue(parAMPL, xy.y);
}

void PseudoVoigt1::setGuessedFWHM(fwhm_t fwhm) {
  super::setGuessedFWHM(fwhm);
  setValue(parSIGMAGAMMA, fwhm / 2);
}

peak_t PseudoVoigt1::fittedPeak() const {
  return peak_t(parameters_.at(parXSHIFT).value(), parameters_.at(parAMPL).value());
}

fwhm_t PseudoVoigt1::fittedFWHM() const {
  return fwhm_t(parameters_.at(parSIGMAGAMMA).value() * 2);
}

peak_t PseudoVoigt1::peakError() const {
  return peak_t(parameters_.at(parXSHIFT).error(), parameters_.at(parAMPL).error());
}

fwhm_t PseudoVoigt1::fwhmError() const {
  return fwhm_t(parameters_.at(parSIGMAGAMMA).error());
}

JsonObj PseudoVoigt1::saveJson() const {
  return super::saveJson().saveString(json_key::TYPE, json_fun_key::PSEUDOVOIGT1);
}

//------------------------------------------------------------------------------

PseudoVoigt2::PseudoVoigt2(qreal ampl, qreal mu, qreal hwhmG, qreal hwhmL,
                           qreal eta) {
  setParameterCount(5);

  auto& parAmpl  = parameters_[parAMPL];
  auto& parMu    = parameters_[parXSHIFT];
  auto& parHwhmG = parameters_[parSIGMA];
  auto& parHwhmL = parameters_[parGAMMA];
  auto& parEta   = parameters_[parETA];

  parAmpl.setValueRange(0, flt_inf);
  parAmpl.setValue(ampl, 0);

  parMu.setValue(mu, 0);

  parHwhmG.setValueRange(0, flt_inf);
  parHwhmG.setValue(hwhmG, 0);

  parHwhmL.setValueRange(0, flt_inf);
  parHwhmL.setValue(hwhmL, 0);

  parEta.setValueRange(0, 1);
  parEta.setValue(eta, 0);
}

qreal PseudoVoigt2::y(qreal x, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal sigma  = parValue(parSIGMA, parValues);
  qreal gamma  = parValue(parGAMMA, parValues);
  qreal eta    = parValue(parETA, parValues);

  qreal argG     = (x - xShift) / sigma;
  qreal argG2    = argG * argG;
  qreal gaussian = ampl * exp(-argG2 * log(2.0));

  qreal argL    = (x - xShift) / gamma;
  qreal argL2   = argL * argL;
  qreal lorentz = ampl / (1 + argL2);

  return (1 - eta) * gaussian + eta * lorentz;
}

qreal PseudoVoigt2::dy(qreal x, uint parIndex, qreal const* parValues) const {
  qreal ampl   = parValue(parAMPL, parValues);
  qreal xShift = parValue(parXSHIFT, parValues);
  qreal sigma  = parValue(parSIGMA, parValues);
  qreal gamma  = parValue(parGAMMA, parValues);
  qreal eta    = parValue(parETA, parValues);

  qreal argG1 = (x - xShift) / sigma;
  qreal argG2 = argG1 * argG1;
  qreal argG3 = exp(-argG2 * log(2.0));

  qreal argL1 = (x - xShift) / gamma;
  qreal argL2 = argL1 * argL1;
  qreal argL3 = 1 + argL2;

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

void PseudoVoigt2::setGuessedPeak(peak_t::rc xy) {
  super::setGuessedPeak(xy);
  setValue(parXSHIFT, xy.x);
  setValue(parAMPL, xy.y);
}

void PseudoVoigt2::setGuessedFWHM(fwhm_t fwhm) {
  super::setGuessedFWHM(fwhm);
  setValue(parSIGMA, fwhm * 0.424661);
  setValue(parGAMMA, fwhm / 2);
}

peak_t PseudoVoigt2::fittedPeak() const {
  return peak_t(parameters_.at(parXSHIFT).value(), parameters_.at(parAMPL).value());
}

fwhm_t PseudoVoigt2::fittedFWHM() const {
  qreal eta = parameters_.at(parETA).value();
  return fwhm_t(((1 - eta) * parameters_.at(parSIGMA).value() / 0.424661 +
                 eta * parameters_.at(parGAMMA).value() * 2) / 2);
}

peak_t PseudoVoigt2::peakError() const {
  return peak_t(parameters_.at(parXSHIFT).error(), parameters_.at(parAMPL).error());
}

fwhm_t PseudoVoigt2::fwhmError() const {
  // REVIEW
  return fwhm_t(parameters_.at(parSIGMA).error() +
                parameters_.at(parGAMMA).error());
}

JsonObj PseudoVoigt2::saveJson() const {
  return super::saveJson().saveString(json_key::TYPE, json_fun_key::PSEUDOVOIGT2);
}

*/

//------------------------------------------------------------------------------
}}
// eof
