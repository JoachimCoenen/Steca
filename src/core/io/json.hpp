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

#pragma once
#include <lib/dev/io/json.hpp>
#include <lib/dev/typ/factory.hpp>

namespace core {

struct Range;
struct Ranges;
struct Fun;

namespace calc {
  struct Reflection;
}

namespace io {
//------------------------------------------------------------------------------

dcl_sub_(Json, l_io::Json)
  using base::base;

  fry_(Json::rc, asSelf, (base::rc that)) may_err;

  mth_(Json::rc, at, (strc key)) may_err  RET_(asSelf(base::at(key)))
  mth_(Json::rc, at, (uint i))   may_err  RET_(asSelf(base::at(i)))
  set_(add, (strc key, rc that)) may_err  SET_(base::add(key, that))
  set_(add, (rc that))           may_err  SET_(base::add(that))

  Json(Range const&);
  mth_(Range, asRange, ()) may_err;

  Json(Ranges const&);
  mth_(Ranges, asRanges, ()) may_err;

  Json(Fun const&) may_err;
  mth_(l::own<Fun>, asFun, ()) may_err;

  Json(calc::Reflection const&);
  mth_(l::own<calc::Reflection>, asReflection, ()) may_err;
dcl_end


//------------------------------------------------------------------------------
}}
