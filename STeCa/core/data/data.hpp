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

#ifndef CORE_DATA_HPP
#define CORE_DATA_HPP

#include "../typ/def.hpp"
#include "image.hpp"
#include <c2/c/tim.h>
#include <c2/cpp/map.hpp>

namespace core {
struct Session;

namespace data {
//------------------------------------------------------------------------------

using flt_vec = c::vec<flt32>;

dcl_struct_(Meta) SHARED // metadata
  // attribute dictionary
  dcl_struct_reimpl_(Dict, c::map<c::str COMMA uint>) SHARED
    using base::size;
    mth_mut_(uint, add, (c::strc))
    mth_err_(uint, at,  (c::strc))
  };

  atr_(c::str, comment)

  atr_(Dict::sh, dict) // other than the values stored explicitly below
  atr_(flt_vec,  vals)

  atr_(tth_t,  tth)    // *mid* tth
  atr_(omg_t,  omg)
  atr_(chi_t,  chi)
  atr_(phi_t,  phi)

  atr_(flt32,  tim)    // time
  atr_(flt32,  mon)    // monitor count
  atr_(flt32, dTim)    // delta time, may be nan
  atr_(flt32, dMon)    // delta mon. count, may be nan

  Meta(Dict::shc);
  Meta(Dict::shc, flt_vec::rc, flt32, flt32, flt32, flt32,
                               flt32, flt32, flt32, flt32);
dcl_end

//------------------------------------------------------------------------------

dcl_struct_(Set) SHARED   // one dataset, as acquired
  atr_(uint,      idx)   // this order in File, 1..; 0 = not in File
  atr_(Meta::sh,  meta)
  atr_(Image::sh, image)

  Set(Meta::sh, Image::sh);

  val_(tth_t::rc, tth, (), meta->tth)
  val_(omg_t::rc, omg, (), meta->omg)
  val_(phi_t::rc, phi, (), meta->phi)
  val_(chi_t::rc, chi, (), meta->chi)

  val_(flt32,     tim, (), meta->tim)
  val_(flt32,     mon, (), meta->mon)
  val_(flt32,    dTim, (), meta->dTim)
  val_(flt32,    dMon, (), meta->dMon)

  mth_(gma_rge,   rgeGma,     (Session const&))
  mth_(gma_rge,   rgeGmaFull, (Session const&))
  mth_(tth_rge,   rgeTth,     (Session const&))

  val_(inten_rge, rgeInten, (), image->rgeInten())

  voi_(collect, (Session const&, Image const* corr,
                  core::inten_vec&, core::uint_vec&, gma_rge::rc,
                  tth_t minTth, tth_t deltaTth))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_(CombinedSet) SHARED   // one or more Set
  atr_(c::vec<Set::sh>, sets)

  CombinedSet();

  mth_(Meta::sh,  meta,  ())
  mth_(Image::sh, image, ())

  // no tth
  mth_(omg_t::rc, omg, ())
  mth_(phi_t::rc, phi, ())
  mth_(chi_t::rc, chi, ())

  mth_(flt32,     tim, ())
  mth_(flt32,     mon, ())
  mth_(flt32,    dTim, ())
  mth_(flt32,    dMon, ())

  mth_(gma_rge,   rgeGma,     (Session const&))
  mth_(gma_rge,   rgeGmaFull, (Session const&))
  mth_(tth_rge,   rgeTth,     (Session const&))

  mth_(inten_rge, rgeInten, ())

private_
  mut_(Meta::sh,  lazyMeta)
  mut_(Image::sh, lazyImage)

  mut_(omg_t,     lazyOmg)
  mut_(phi_t,     lazyPhi)
  mut_(chi_t,     lazyChi)

  mut_(flt32,     lazyTim)
  mut_(flt32,     lazyMon)
  mut_(flt32,     lazyDTim)
  mut_(flt32,     lazyDMon)

  mth_(inten_vec, collect, (Session const&, Image const* corr, gma_rge::rc))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_(CombinedSets) SHARED
  atr_(c::vec<CombinedSet::sh>, sets)

  CombinedSets();
dcl_end

//------------------------------------------------------------------------------

struct Files;

dcl_struct_(File) SHARED  // one file
  ref_(Files, files) atr_(uint, idx) // this order in Files, 1..; 0 = not in Files
  atr_(c::str, name)
  atr_(c::str, comment)
  atr_(c::vec<std::pair<c::str COMMA c::str>>, strs)
  atr_(c::vec<Set::sh>, sets)

  File(Files const&, c::strc name);

  voi_mut_(addSet, (Set::sh))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_(Files) SHARED // the whole file group
  atr_(c::vec<File::sh>, files)
  atr_(Meta::Dict::sh,   dict)

  Files();

  voi_mut_(addFile, (data::File::sh))
  voi_mut_(remFile, (uint))
dcl_end

//------------------------------------------------------------------------------
}}
#endif
