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

#include "json.hpp"
#include <dev_lib/defs.inc>
#include "../typ/fun.hpp"
#include "../fit/fit_fun.hpp"
#include "../fit/fit.hpp"

namespace core { namespace io {
//------------------------------------------------------------------------------

namespace key {

str const
  MIN, MAX,
  PARAMS, TYPE, FUN, SUM,
  VALUE, RANGE, COUNT,
  PEAK, FWHM,
  POLYNOM("polynom"), RAW("Raw"),
  GAUSSIAN("Gaussian"), LORENTZIAN("Lorentzian"),
  PSEUDOVOIGT1("PseudoVoigt1"), PSEUDOVOIGT2("PseudoVoigt2");
}

//------------------------------------------------------------------------------

Json::Json(Range::rc v) : base(OBJ) {
  add(key::MIN, Json(v.min));
  add(key::MAX, Json(v.max));
}

Range Json::asRange() const {
  return Range(at(key::MIN).asFlt(), at(key::MAX).asFlt());
}

Json::Json(Ranges::rc v) : base(VEC) {
  for_i_(v.size())
    add(Json(v.at(i)));
}

Ranges Json::asRanges() const {
  Ranges res;
  for_i_(size())
    res.add(at(i).asRange());
  return res;
}

Json::Json(Par::rc v) : base(OBJ) {
  add(key::VALUE, Json(v.val));
  add(key::RANGE, Json(Range()));
}

Par Json::asPar() const {
  return Par(at(key::VALUE).asFlt(), at(key::RANGE).asFlt());
}

Json::Json(SimpleFun::rc v) : base(OBJ) {
  Json::Vec ps;
  for (auto& par : v.pars)
    ps.add(Json(par));

  add(key::PARAMS, Json(ps));
}

void Json::load(SimpleFun& fun) const may_err {
  Json ps = at(key::PARAMS);
  for_i_(ps.size())
    fun.add(ps.at(i).asPar());
}

Json::Json(SumFuns::rc v) : base(OBJ) {
  add(key::TYPE, Json(key::SUM));

  uint funCount = v.funs.size();
  add(key::COUNT, Json(funCount));

  for_i_(funCount)
    add(CAT(key::FUN, (i + 1)), save(*(v.funs.at(i))));
}

void Json::load(SumFuns& fun) const may_err {
  check_or_err_(fun.funs.isEmpty(), "cannot load twice");
  uint funCount = at(key::COUNT).asUint();

  for_i_(funCount)
    fun.add(make(at(CAT(key::FUN, (i+1))).asObj()));
}

Json(core::fit::Polynom::rc pol) : base(OBJ) {
  JsonObj obj;
  obj.saveString(json_key::TYPE, json_fun_key::POLYNOM);
  return super::saveJson() + obj;
}

void Json::load(fit::Polynom&) const may_err {

}

Json::Json(fit::PeakFun::rc) {

}

void Json::load(fit::Polynom&) const may_err {

}

//------------------------------------------------------------------------------
}}
// eof
