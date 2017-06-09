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

#ifndef CORE_FUN_H
#define CORE_FUN_H

#include "def.h"
#include "factory.h"

namespace core {
//------------------------------------------------------------------------------

namespace key {
extern str const
  I, J, X, Y, MIN, MAX,
  PARAMS, TYPE, FUN, SUM,
  VALUE, RANGE, COUNT;
}

dcl_(Par)
  atr_(real, val)
  atr_(real, err)

  Par();
  Par(real, real);

  void set(real val, real err);
  void setVal(real);

  ref operator=(rc);
dcl_end

dcl_(Fun) SHARED
  Fun();
  virtual ~Fun();

  virtual sz_t parCount()     const = 0;
  virtual Par::rc parAt(sz_t) const = 0;
  virtual void resetPars()    {}

  // evaluate the fun y = f(x), with given (parVals) or own pars
  virtual real y(real x, real const* parVals = nullptr) const = 0;

  // partial derivative / par, with given (parVals) or own pars
  virtual real dy(real x, sz_t parIdx, real const* parVals = nullptr) const = 0;

  using fryFun = factory<Fun>;

protected:
  static fryFun funFry;

public:
  static void initFry();
  static void addMaker(strc key, l::give_me<fryFun::maker_base>);
  static l::own<Fun> make(strc key)       may_err;
dcl_end

dcl_sub_(SimpleFun, Fun)
  atr_(QVector<Par>, pars)

  SimpleFun();

  void    add(Par::rc);

  sz_t    parCount()  const;
  Par::rc parAt(sz_t) const;
  void    resetPars();

  real    parVal(sz_t parIdx, real const* parVals) const;
  void    setParVal(sz_t parIdx, real val);
dcl_end

// a fun that is a sum of other funs

dcl_sub_(SumFuns, Fun)
  void add(l::give_me<Fun>);

  // aggregate par list for all added funs
  sz_t    parCount()  const;
  Par::rc parAt(sz_t) const;

  real  y(real x, real const* parVals = nullptr)              const;
  real dy(real x, sz_t parIdx, real const* parVals = nullptr) const;

  // summed funs
  QVector<Fun::sh> funs;

protected:
  // the aggregate par list
  QVector<Par const*> allPars;
  // look up the original fun for a given aggregate par index
  QVector<Fun const*> fun4parIdx;
  // the starting index of pars of a summed fun, given the aggregate par index
  QVector<sz_t> firstParIdx4parIdx;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
