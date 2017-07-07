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

#pragma once

#include <dev_lib/io/json.hpp>

namespace core {

struct Range;
struct Ranges;

struct Par;
struct Fun;
struct SimpleFun;
struct SumFuns;

namespace fit {
struct Polynom;
struct PeakFun;
}

namespace io {
//------------------------------------------------------------------------------

dcl_sub_(Json, l_io::Json)
  using base::base;

  static Json::rc asSelf(base::rc that)
    VAL_(static_cast<Self::rc>(that))

  mth_(Json::rc, at, (strc key)) may_err
    VAL_(asSelf(base::at(key)))
  mth_(Json::rc, at, (uint i)) may_err
    VAL_(asSelf(base::at(i)))

  Json(Range const&);
  mth_(Range, asRange, ()) may_err;

  Json(Ranges const&);
  mth_(Ranges, asRanges, ()) may_err;

  Json(Par const&);
  mth_(Par, asPar, ()) may_err;

  fry_(Json, save, (Fun const&)) may_err;

  Json(SimpleFun const&);
  mth_(void, load, (SimpleFun&)) may_err;

  Json(SumFuns const&);
  mth_(void, load, (SumFuns&)) may_err;

  Json(fit::Polynom const&);
  mth_(void, load, (fit::Polynom&)) may_err;

  fry_(Json, save, (fit::PeakFun const&));
  mth_(void, load, (fit::PeakFun&)) may_err;
dcl_end

//------------------------------------------------------------------------------
}}
