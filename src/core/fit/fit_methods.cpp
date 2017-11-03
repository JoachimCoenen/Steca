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

#include "fit_methods.hpp"
#include <lib/dev/inc/defs.inc>
#include "LM/levmar.h"

namespace core { namespace fit {
//------------------------------------------------------------------------------

Method::~Method() {}

Method::Method() : fun(nullptr), xs(nullptr) {}

void Method::fit(Fun& f, Curve::rc curve) const {
  if (curve.isEmpty())
    return;

  mut(fun) = &f;
  mut(xs)  = &curve.xs;

  // prepare data in a required format
  auto parCount = fun->parCount();
  real_vec parVal(parCount), parMin(parCount), parMax(parCount), parErr(parCount);

  for_i_(parCount) {
    auto par = fun->parAt(i);
    parVal.setAt(i, par.val);
    parMin.setAt(i, -l::real_inf); // it is possible to extend Par by allowed range
    parMax.setAt(i, +l::real_inf);  // and retrieve from there
  }

  approximate(parVal.data(), parMin.data(), parMax.data(),
              parErr.data(), parCount, curve.ys.data(), curve.size());

  // read data
  for_i_(parCount)
    mut(fun->parAt(i)).set(parVal.at(i), parErr.at(i));
}

void Method::callbackY(real* parVals, real* yVals, int /*parCount*/, int xSize, void*) const {
  for_i_(xSize)
    yVals[i] = fun->y(xs->at(i), parVals);
}

//------------------------------------------------------------------------------

void LinearLeastSquare::approximate(
    real* parVals,        // IO initial estimates -> estimated solution
    real const* parMins,  // I
    real const* parMaxs,  // I
    real*       parErrs,  // O
    uint        parCount, // I
    real const* yVals,    // I
    uint        valCount) // I
const {
  // output
  double info[LM_INFO_SZ];              // info regarding the minimization
  real_vec covar(parCount * parCount);  // covariance matrix

  uint const maxIterations = 1000;
  DelegateCalculationDbl dlg(this, &Self::callbackY);

  // here mut() to satisfy the lack of const in LevMar headers
  dlevmar_bc_dif(&dlg, parVals, mutp(yVals), parCount, valCount,
                 mutp(parMins), mutp(parMaxs), nullptr,
                 maxIterations, nullptr, info, nullptr, covar.data(), nullptr);

  for_i_(parCount)
    parErrs[i] = sqrt(covar.at(i * parCount + i));  // the diagonal
}

//------------------------------------------------------------------------------

void LevenbergMarquardt::approximate(
    real* parVals,        // IO initial estimates -> estimated solution
    real const* parMins,  // I
    real const* parMaxs,  // I
    real*       parErrs,  // O
    uint        parCount, // I
    real const* yVals,    // I
    uint        valCount) // I
const {
  // minim. options mu, epsilon1, epsilon2, epsilon3
  double opts[LM_OPTS_SZ];
  opts[0] = LM_INIT_MU;
  opts[1] = 1e-12;
  opts[2] = 1e-12;
  opts[3] = 1e-18;

  // output
  double info[LM_INFO_SZ];              // info regarding the minimization
  real_vec covar(parCount * parCount);  // covariance matrix

  uint const maxIterations = 1000;

  DelegateCalculationDbl dlg(this, &Self::callbackY);
  DelegateCalculationDbl dlgJacobian(this, &Self::callbackJacobianLM);

      // here mut() to satisfy the lack of const in LevMar headers
  dlevmar_bc_der(&dlg, &dlgJacobian, parVals, mutp(yVals), parCount, valCount,
                 mutp(parMins), mutp(parMaxs), nullptr,
                 maxIterations, opts, info, nullptr, covar.data(), nullptr);

  for_i_(parCount)
    parErrs[i] = sqrt(covar.at(i * parCount + i));  // the diagonal
}

void LevenbergMarquardt::callbackJacobianLM(
    real* parVals, real* jacobian, int parCount, int xSize, void*) const
{
  for_var_(ix, xSize)
    for_var_(ip, parCount)
      *jacobian++ = fun->dy(xs->at(ix), ip, parVals);
}

//------------------------------------------------------------------------------
}}
// eof
