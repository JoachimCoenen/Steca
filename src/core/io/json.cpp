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
#include <lib/dev/inc/defs.inc>
#include "../typ/fun.hpp"
#include "../fit/fit_fun.hpp"
#include "../calc/reflection.hpp"

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

Json::rc Json::asSelf(base::rc that) may_err {
  check_or_err_(dynamic_cast<Self const*>(&that), "bad Jason");
  return static_cast<Self::rc>(that);
}

Json::Json(Range::rc v) : base(OBJ) {
  add(key::MIN, Json(v.min));
  add(key::MAX, Json(v.max));
}

Range Json::asRange() const may_err {
  return Range(at(key::MIN).asFlt(), at(key::MAX).asFlt());
}

Json::Json(Ranges::rc v) : base(VEC) {
  for_i_(v.size())
    add(Json(v.at(i)));
}

Ranges Json::asRanges() const may_err {
  Ranges res;
  for_i_(size())
    res.add(at(i).asRange());
  return res;
}

//------------------------------------------------------------------------------
// functions

static Json jsonObj(Par::rc v) {
  return Json(Json::OBJ)
          .add(key::VALUE, Json(v.val))
          .add(key::RANGE, Json(Range()));
}

static Par asPar(Json::rc obj) {
  return Par(obj.at(key::VALUE).asFlt(), obj.at(key::RANGE).asFlt());
}

template <typename T, typename O> T const* isType(O const* o) {
  return dynamic_cast<T const*>(o);
}

template <typename T, typename O> T const& asType(O const& o) {
  ENSURE_(isType<T>(&o))
  return static_cast<T const&>(o);
}

Json::Json(Fun const& fun) may_err : base(OBJ) {
  if (isType<SumFuns>(&fun)) {

    auto& f = asType<SumFuns>(fun);

    add(key::TYPE, Json(key::SUM));

    uint funCount = f.funs.size();
    add(key::COUNT, Json(funCount));

    for_i_(funCount)
      add(CAT(key::FUN, (i + 1)), Json(*(f.funs.at(i))));

  } else if (isType<SimpleFun>(&fun)) {

    auto& f = asType<SimpleFun>(fun);

    if (isType<fit::Polynom>(&fun)) {

      add(key::TYPE, Json(key::POLYNOM));

    } else if (isType<fit::PeakFun>(&fun)) {

      auto& f = asType<fit::PeakFun>(fun);

      if (isType<fit::Raw>(&f))
        add(key::TYPE, Json(key::RAW));
      else if (isType<fit::Gaussian>(&f))
        add(key::TYPE, Json(key::GAUSSIAN));
      else if (isType<fit::Lorentzian>(&f))
        add(key::TYPE, Json(key::LORENTZIAN));
      else if (isType<fit::PseudoVoigt1>(&f))
        add(key::TYPE, Json(key::PSEUDOVOIGT1));
      else if (isType<fit::PseudoVoigt2>(&f))
        add(key::TYPE, Json(key::PSEUDOVOIGT2));
      else
        l::err("bad PeakFun");

      add(key::RANGE, Json(f.range));
      add(key::PEAK,  Json(f.guessedPeak));
      add(key::FWHM,  Json(f.guessedFWHM));

    } else
      l::err("bad SimpleFun");

    Json::Vec ps;
    for (auto& par : f.pars)
      ps.add(jsonObj(par));

    add(key::PARAMS, Json(ps));

  } else
    l::err("bad Fun");
}

l::own<Fun> Json::asFun() const may_err {
  str type = at(key::TYPE).asStr();

  if (key::SUM == type) {
    auto f = l::scope(new SumFuns);

    uint funCount = at(key::COUNT).asUint();
    for_i_(funCount)
      f->add(at(CAT(key::FUN, (i+1))).asFun());

    return f.takeOwn();

  } else { // SimpleFun

    auto loadSimpleFun = [this](SimpleFun& f) {
      Json ps = at(key::PARAMS);
      for_i_(ps.size())
        f.add(asPar(ps.at(i)));
    };

    if (key::POLYNOM == type) {

      auto f = l::scope(new fit::Polynom);
      loadSimpleFun(*f);
      return f.takeOwn();

    } else { // PeakFun

      auto loadPeakFun = [this, &loadSimpleFun](fit::PeakFun& f) {
        loadSimpleFun(f);
        mut(f.range)       = at(key::RANGE).asRange();
        mut(f.guessedPeak) = at(key::PEAK).asXY();
        mut(f.guessedFWHM) = at(key::FWHM).asFlt();
      };

      if (key::RAW == type) {
        auto f = l::scope(new fit::Raw);
        loadPeakFun(*f);
        return f.takeOwn();
      }

      if (key::GAUSSIAN == type) {
        auto f = l::scope(new fit::Gaussian);
        loadPeakFun(*f);
        return f.takeOwn();
      }

      if (key::LORENTZIAN == type) {
        auto f = l::scope(new fit::Lorentzian);
        loadPeakFun(*f);
        return f.takeOwn();
      }

      if (key::PSEUDOVOIGT1 == type) {
        auto f = l::scope(new fit::PseudoVoigt1);
        loadPeakFun(*f);
        return f.takeOwn();
      }

      if (key::PSEUDOVOIGT2 == type) {
        auto f = l::scope(new fit::PseudoVoigt2);
        loadPeakFun(*f);
        return f.takeOwn();
      }

      l::err("bad PeakFun");

    }
  }
}

Json::Json(calc::Reflection::rc r) : Json(*r.peakFun) {}

l::own<calc::Reflection> Json::asReflection() const may_err {
  auto fun = l::scope(asFun());
  check_or_err_(isType<fit::PeakFun>(fun.ptr()), "must be a peak function");

  auto r = l::scope(new calc::Reflection);
  auto p = static_cast<fit::PeakFun const*>(fun.take().ptr());
  mut(r->peakFun).reset(p);
  return r.takeOwn();
}

//------------------------------------------------------------------------------
}}
// eof
