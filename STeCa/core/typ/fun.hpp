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

#include <c/c/h_data>
#include <c/c/lib/num.h>
#include <c/cpp/vec.hpp>
#include "fry.hpp"

namespace core {
//------------------------------------------------------------------------------

struct JsonObj;

struct Fun { _typedefs(Fun)

  struct Par final { _typedefs(Par)
    _atr(real, val)
    _atr(real, err)

    Par(real, real);
    void set(real, real);
    void setVal(real);

    ref operator=(rc);

    JsonObj    toJson() const;
    static Par fromJson(JsonObj const&) may_exc;
  };

  Fun();
  virtual ~Fun();

  virtual sz_t parCount()     const = 0;
  virtual Par::rc parAt(sz_t) const = 0;
  virtual void resetPars()    {}

  // evaluate the fun y = f(x), with given (parVals) or own pars
  virtual real y(real x, real const* parVals = nullptr) const = 0;

  // partial derivative / par, with given (parVals) or own pars
  virtual real dy(real x, sz_t parIdx, real const* parVals = nullptr) const = 0;

  typedef fry<Fun> fryFun;

protected:
  static fryFun factory;

public:
  static void addMaker(strc key, c::give_me<fryFun::someMaker const>);
  static c::own<Fun> make(strc key)       may_exc;
  static c::own<Fun> make(JsonObj const&) may_exc;

  virtual JsonObj toJson() const;
  virtual void    loadJson(JsonObj const&) may_exc;
};

typedef c::shared<Fun> shFun;

struct SimpleFun : Fun { _typedefs(SimpleFun) using base = Fun;
  _atr(c::vec<Par>, pars)

  SimpleFun();

  void    add(Par::rc);

  sz_t    parCount()  const;
  Par::rc parAt(sz_t) const;
  void    resetPars();

  real    parVal(sz_t parIdx, real const* parVals) const;
  void    setParVal(sz_t parIdx, real val);

  JsonObj toJson() const;
  void    loadJson(JsonObj const&) may_exc;
};

// a fun that is a sum of other funs

struct SumFuns final : Fun { _typedefs(SumFuns) using base = Fun;
  void add(c::give_me<Fun>);

  // aggregate par list for all added funs
  sz_t    parCount()  const;
  Par::rc parAt(sz_t) const;

  real  y(real x, real const* parVals = nullptr)              const;
  real dy(real x, sz_t parIdx, real const* parVals = nullptr) const;

  JsonObj toJson() const;
  void    loadJson(JsonObj const&) may_exc;

protected:
  // summed funs
  c::vec<shFun>      funs;
  // the aggregate par list
  c::vec<Par const*> allPars;
  // look up the original fun for a given aggregate par index
  c::vec<Fun const*> fun4parIdx;
  // the starting index of pars of a summed fun, given the aggregate par index
  c::vec<sz_t> firstParIdx4parIdx;
};

//------------------------------------------------------------------------------
}
#endif
// eof
