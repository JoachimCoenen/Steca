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

#ifndef TYP_FUN_H
#define TYP_FUN_H

#include "def/defs.h"
#include "typ/typ_factory.h"
#include "typ/typ_json.h"
#include "typ/typ_range.h"

namespace json_fun_key {
extern str const
  SUM;
}

namespace typ {
//------------------------------------------------------------------------------
// Abstract function

class Function {
  CLASS(Function)
public:
  class Factory : public typ::Factory<Function> {
    CLASS(Factory) SUPER(typ::Factory<Function>)
  public:
    owner_not_null<Function*> make(JsonObj::rc) may_exc;
  };

protected:
  static Factory factory_;

public:
  static void addFactoryMaker(qstrc key, owner_not_null<Factory::MakerBase*>);
  static void initFactory();

  static owner_not_null<Function*> make(JsonObj::rc) may_exc;

public:
  class Parameter final {
    CLASS(Parameter)
  public:
    Parameter();

    real value() const { return value_; }
    real error() const { return error_; }

    Range valueRange() const;  // allowed range of values
    void  setValueRange(real min, real max);

    void  setValue(real value, real error);

  public:
    JsonObj saveJson() const;
    void loadJson(JsonObj::rc) may_exc;

  private:
    real value_, error_;

    // allowed range of values
    Range range_;
  };

public:
  Function();
  virtual ~Function() {}

  virtual uint parameterCount() const = 0;

  virtual Parameter&    parameterAt(uint) = 0;
  virtual Parameter::rc parameterAt(uint i) const {
    return const_cast<Function*>(this)->parameterAt(i);
  }

  // evaluate the function y = f(x), with given (parValues) or own parameters
  virtual real y(real x, real const* parValues = nullptr) const = 0;

  // partial derivative / parameter, with given (parValues) or own parameters
  virtual real dy(real x, uint parIndex,
                   real const* parValues = nullptr) const = 0;
public:
  virtual JsonObj saveJson() const;
  virtual void loadJson(JsonObj::rc) may_exc;
};

//------------------------------------------------------------------------------
// abstract function with parameters

class SimpleFunction : public Function {
  CLASS(SimpleFunction) SUPER(Function)
public:
  SimpleFunction();

  void       setParameterCount(uint);
  uint       parameterCount() const;
  Parameter& parameterAt(uint);

  virtual void reset();

public:
  JsonObj saveJson() const;
  void loadJson(JsonObj::rc) may_exc;

protected:
  vec<Parameter> parameters_;
  real parValue(uint parIndex, real const* parValues) const;
  void  setValue(uint parIndex, real val);
};

//------------------------------------------------------------------------------
// concrete function that is a sum of other functions

class SumFunctions final : public Function {
  CLASS(SumFunctions) SUPER(Function)
public:
  SumFunctions();
 ~SumFunctions();

  void addFunction(owner_not_null<Function*>);

  // aggregate parameter list for all added functions
  uint       parameterCount() const;
  Parameter& parameterAt(uint);

  real y(real x, real const* parValues = nullptr) const;
  real dy(real x, uint parIndex, real const* parValues = nullptr) const;

public:
  JsonObj saveJson() const;
  void loadJson(JsonObj::rc) may_exc;

protected:
  // summed functions
  vec<Function*> functions_;
  // the aggregate parameter list
  vec<Parameter*> allParameters_;
  // look up the original function for a given aggregate parameter index
  vec<Function*> function4parIndex_;
  // the starting index of parameters of a summed function, given the aggregate
  // parameter index
  uint_vec firstParIndex4parIndex_;
};

//------------------------------------------------------------------------------
}
#endif // TYP_FUN_H
