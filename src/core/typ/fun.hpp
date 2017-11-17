/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

/** @file
 * A representation of functions and their combination.
 */

#pragma once

#include <lib/dev/inc/ptr.hpp>
#include <lib/dev/typ/vec.hpp>

namespace core {
//------------------------------------------------------------------------------

/// Function parameter
dcl_(Par)
  atr_(real, val);  ///< parameter value
  atr_(real, err);  ///< parameter error (i.e. standard deviation)

  Par(real val = 0, real err = 0);

  set_(set, (real));                ///< set the value
  set_(set, (real val, real err));  ///< set the value and error

  set_(operator=, (rc));
dcl_end

/// An abstract function
dcl_base_(Fun) SHARED
  Fun();

  virtual mth_(uint, parCount, ())     = 0; ///< the number of parameters
  virtual mth_(Par::rc, parAt, (uint)) = 0; ///< access to n-th parameter

  /** Evaluate y = f(x); if @c parVals is given, parametr values are taken
   * from it <0..parCount), otherwise @c parAt calls are used
   */
  virtual mth_(real, y, (real x, real const* parVals = nullptr)) = 0;

  /// partial derivative dy = f(x) | parIdx; with given @c parVals or @c parAt
  virtual mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr)) = 0;
dcl_end

/// An abstract simple function (stores parameters)
dcl_sub_(SimpleFun, Fun)
  atr_(l::vec<Par>, pars);        ///< parameters

  SimpleFun();

  set_(reset, ());                ///< clear all parameters

  set_(setParCount, (uint));      ///< allocate parameters (cleared)
  set_(add, (Par::rc));           ///< extend the parameters by one

  mth_(uint,    parCount, ());
  mth_(Par::rc, parAt, (uint));

  /// get a @c parIdx-th parameter's value; either from @c parVals or from own
  mth_(real, parVal, (uint parIdx, real const* parVals = nullptr));
  /// set own parameter's value
  set_(setParVal, (uint parIdx, real val));
  /// get a parameter's error
  mth_(real, parErr, (uint parIdx));
dcl_end

/** A sum of functions (purpose: eventual option to fit overlapped peaks);
 * the aggregate parameters is a sequence of parameters of individual functions
 */
dcl_sub_(SumFuns, Fun)
  atr_(l::vec<Fun::shp>, funs);   ///< a collection of summed functions
  set_(add, (l::give_me<Fun>));   ///< add a function to the collection

  mth_(uint,    parCount, ());    ///< the aggregate parameter count
  mth_(Par::rc, parAt, (uint));   ///< access to aggregate parameter list

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

protected:
  /// the aggregate parameter list - pointers to parameters in individual functions
  atr_(l::vec<Par const*>, allPars);
  /// given an aggregate parameter index, look up the owning individual function
  atr_(l::vec<Fun const*>, fun4parIdx);
  /// given an aggregate parameter index, look the index of the first parameter of the same individual function
  atr_(l::vec<uint>, firstParIdx4parIdx);
dcl_end

//------------------------------------------------------------------------------
}
// eof
