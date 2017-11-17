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
  virtual set_(resetPars, ())  SET_()

  // evaluate the fun y = f(x), with given (parVals) or own pars
  virtual mth_(real, y, (real x, real const* parVals = nullptr)) = 0;

  // partial derivative / par, with given (parVals) or own pars
  virtual mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr)) = 0;
dcl_end

dcl_sub_(SimpleFun, Fun)
  atr_(l::vec<Par>, pars);

  SimpleFun();

  set_(reset, ());

  set_(setParCount, (uint));
  set_(add, (Par::rc));

  mth_(uint,    parCount, ());
  mth_(Par::rc, parAt, (uint));
  set_(resetPars, ());

  mth_(real, parVal, (uint parIdx, real const* parVals = nullptr));
  set_(setParVal, (uint parIdx, real val));

  mth_(real, parErr, (uint parIdx));
dcl_end

// a fun that is a sum of other funs

dcl_sub_(SumFuns, Fun)
  set_(add, (l::give_me<Fun>));

  // aggregate par list for all added funs
  mth_(uint,    parCount, ());
  mth_(Par::rc, parAt, (uint));

  mth_(real,  y, (real x, real const* parVals = nullptr));
  mth_(real, dy, (real x, uint parIdx, real const* parVals = nullptr));

  // summed funs
  l::vec<Fun::shp> funs;

protected:
  // the aggregate par list
  l::vec<Par const*> allPars;
  // look up the original fun for a given aggregate par index
  l::vec<Fun const*> fun4parIdx;
  // the starting index of pars of a summed fun, given the aggregate par index
  l::vec<uint> firstParIdx4parIdx;
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
