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
#include <dev_lib/inc/defs.inc>

namespace core {
//------------------------------------------------------------------------------

Par::Par(real val_, real err_) : val(val_), err(err_) {}

Par::ref Par::set(real val_, real err_) {
  mut(val) = val_;
  mut(err) = err_;
  RTHIS
}

Par::ref Par::setVal(real val_)
  SET_(mut(val) = val_)

Par::ref Par::operator=(rc that) {
  mut(val) = that.val;
  mut(err) = that.err;
  RTHIS
}

//------------------------------------------------------------------------------

Fun::Fun()  {}
Fun::~Fun() {}

//------------------------------------------------------------------------------

SimpleFun::SimpleFun() : pars() {}

SimpleFun::ref SimpleFun::reset() {
  for (auto& par : pars)
    mut(par) = Par(0, 0);
  RTHIS
}

SimpleFun::ref SimpleFun::setParCount(uint cnt) {
  mut(pars).clear();
  for_i_(cnt)
    mut(pars).add(Par(0,0));
  RTHIS
}

SimpleFun::ref SimpleFun::add(Par::rc par)
  SET_(mut(pars).add(par))

sz_t SimpleFun::parCount() const {
  return pars.size();
}

Par::rc SimpleFun::parAt(sz_t i) const {
  return pars.at(i);
}

SimpleFun::ref SimpleFun::resetPars() {
  for_i_(parCount())
    mut(pars).setAt(i, Par(0, 0));
  RTHIS
}

real SimpleFun::parVal(sz_t i, real const* parVals) const {
  return parVals ? parVals[i] : pars.at(i).val;
}

SimpleFun::ref SimpleFun::setParVal(sz_t i, real val)
  SET_(mut(pars.at(i)).setVal(val))

real SimpleFun::parErr(sz_t i) const {
  return pars.at(i).err;
}

//------------------------------------------------------------------------------

SumFuns::ref SumFuns::add(l::give_me<Fun> fun) {
  sz_t parIdx = parCount();
  funs.add(Fun::sh(fun.ptr()));

  for_i_(fun->parCount()) {
    // aggregate par list
    allPars.add(&fun->parAt(i));
    // lookup helpers
    fun4parIdx.add(fun);
    firstParIdx4parIdx.add(parIdx);
  }

  RTHIS
}

sz_t SumFuns::parCount() const {
  return allPars.size();
}

Par::rc SumFuns::parAt(sz_t i) const {
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

  ENSURE_(firstIndex <= i)
  i -= firstIndex;

  ENSURE_(i < f->parCount())
      return f->dy(x, i, parVals);
}

//------------------------------------------------------------------------------
}
// eof
