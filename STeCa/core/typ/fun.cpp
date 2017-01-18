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

#include "fun.hpp"
#include <c/c/cpp>

namespace core {
//------------------------------------------------------------------------------

Fun::par::par(real val_, real err_) : val(val_), err(err_) {}

void Fun::par::set(real val_, real err_) {
  mut(val) = val_;
  mut(err) = err_;
}

void Fun::par::setVal(real val_) {
  mut(val) = val_;
}

Fun::par::ref Fun::par::operator=(rc that) {
  mut(val) = that.val;
  mut(err) = that.err;
  return *this;
}

//------------------------------------------------------------------------------

Fun::Fun()  {}
Fun::~Fun() {}

//------------------------------------------------------------------------------

SimpleFun::SimpleFun() {}

void SimpleFun::add(par::rc par) {
  mut(pars).push(par);
}

sz_t SimpleFun::parCount() const {
  return pars.size();
}

Fun::par::rc SimpleFun::parAt(sz_t i) const {
  return pars.at(i);
}

void SimpleFun::resetPars() {
  for_i (parCount())
    mut(pars).setAt(i, Fun::par(0, 0));
}

real SimpleFun::parVal(sz_t i, real const* parVals) const {
  return parVals ? parVals[i] : pars.at(i).val;
}

void SimpleFun::setParVal(sz_t i, real val) {
  mut(pars.at(i)).setVal(val);
}

//------------------------------------------------------------------------------

void SumFuns::add(c::give_me<Fun> fun) {
  sz_t parIdx = parCount();
  funs.push(shFun(fun));

  for_i (fun->parCount()) {
    // aggregate par list
    allPars.push(&fun->parAt(i));
    // lookup helpers
    fun4parIdx.push(fun);
    firstParIdx4parIdx.push(parIdx);
  }
}

sz_t SumFuns::parCount() const {
  return allPars.size();
}

Fun::par::rc SumFuns::parAt(sz_t i) const {
  return *allPars.at(i);
}

real SumFuns::y(real x, real const* parVals) const {
  real sum = 0;

  for (auto& f : funs) {
    sum += f->y(x, parVals);

    if (parVals)
      parVals += f->parCount();  // advance to next fun
  }

  return sum;
}

real SumFuns::dy(real x, sz_t i, real const* parVals) const {
  auto& f = fun4parIdx.at(i);  // aggregate parIdx refers to fun f

  // offset par index
  sz_t firstIndex = firstParIdx4parIdx.at(i);
  if (parVals) parVals += firstIndex;

  ENSURE(firstIndex <= i)
  i -= firstIndex;

  ENSURE(i < f->parCount())
  return f->dy(x, i, parVals);
}

//------------------------------------------------------------------------------
}
// eof

/*

namespace json_fun_key {
str const
  SUM("sum");
}

namespace typ {
//------------------------------------------------------------------------------

owner_not_null<fun*> fun::Factory::make(JsonObj::rc obj) may_exc {
  str funType = obj.loadString(json_key::TYPE);
  fun *fun = super::make(funType);

  RUNTIME_CHECK(fun, "factory does not know " % funType);

  scoped<fun*> f(fun);
  fun->loadJson(obj); // may throw
  return owner_not_null<fun*>::from(f.take());
}

fun::Factory fun::factory_;

void fun::addFactoryMaker(qstrc key, owner_not_null<Factory::MakerBase*> maker) {
  factory_.addMaker(key, maker);
}

void fun::initFactory() {
  ONLY_ONCE

  addFactoryMaker(json_fun_key::SUM,
      owner_not_null<Factory::MakerBase*>::from(new Factory::Maker<Sumfuns>));
}

owner_not_null<fun*> fun::make(JsonObj::rc obj) {
  return factory_.make(obj);
}


//------------------------------------------------------------------------------

JsonObj Sumfuns::saveJson() const {
  JsonObj obj;
  obj.saveString(json_key::TYPE, json_fun_key::SUM);

  uint funCount = funs_.count();
  obj.saveUint(json_key::COUNT, funCount);

  for_i (funCount)
    obj.saveObj(json_key::FUN.arg(i + 1), funs_.at(i)->saveJson());

  return super::saveJson() + obj;
}

void Sumfuns::loadJson(JsonObj::rc obj) may_exc {
  RUNTIME_CHECK(funs_.isEmpty(),
                "non-empty sum of funs; cannot load twice");

  super::loadJson(obj);

  uint funCount = obj.loadUint(json_key::COUNT);

  for_i (funCount) {
    auto funObj = obj.loadObj(json_key::FUN.arg(i + 1));
    addfun(make(funObj));
  }
}

//------------------------------------------------------------------------------
}
// eof
*/
