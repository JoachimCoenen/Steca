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

#include "reflection.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

Reflection::Reflection(fit::PeakFun::eType type) : peakFun(new fit::Raw) {
  setType(type);
}

Reflection::Reflection(l::give_me<fit::PeakFun const> f) : peakFun(f) {}

void Reflection::setType(fit::PeakFun::eType type) {
  setPeakFun(type);
}

void Reflection::setRange(Range::rc r) {
  mut(*peakFun).setRange(r);
}

void Reflection::setGuess(peak_t::rc p, fwhm_t f) {
  mut(*peakFun).setGuess(p, f);
}

void Reflection::setPeakFun(fit::PeakFun::eType type) {
  Range r =   mut(*peakFun).range;
  setPeakFun(fit::PeakFun::factory(type));
  setRange(r);
}

void Reflection::setPeakFun(l::give_me<fit::PeakFun> f) {
  mut(peakFun).reset(mutp(f.ptr())); // TODO mutp?
}

//rcstr Reflection::typeTag(fit::ePeakType type) {
//  return typeStrLst().at(uint(type));
//}

//fit::ePeakType Reflection::type() const {
//  return peakFunction_->type();
//}

//fit::PeakFunction::rc Reflection::peakFunction() const {
//  ENSURE_(peakFunction_)
//  return *peakFunction_;
//}

//typ::Range::rc Reflection::range() const {
//  return peakFunction_->range();
//}

//void Reflection::fit(typ::Curve::rc curve) {
//  peakFunction_->fit(curve);
//}


//typ::JsonObj Reflection::saveJson() const {
//  return peakFunction_->saveJson();
//}

//void Reflection::loadJson(typ::JsonObj::rc obj) THROWS {
//  scoped<typ::Function*> f(typ::Function::make(obj));

//  RUNTIME_CHECK(dynamic_cast<fit::PeakFunction*>(f.ptr()),
//                "must be a peak function");
//  setPeakFunction(static_cast<fit::PeakFunction*>(f.take()));
//}

//------------------------------------------------------------------------------
}}
// eof
