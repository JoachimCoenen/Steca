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

dcl_struct (Meta) SHARED // metadata
  // attribute dictionary
  dcl_struct_reimpl (Dict, c::map<c::str COMMA uint>) SHARED
    using base::size;
    _mth_mut (uint, add, (c::strc))
    _mth_err (uint, at,  (c::strc))
  };

  _atr (Dict::sh, dict) // other than the values stored explicitly below
  _atr (flt_vec,  vals)

  _atr (tth_t,  tth)    // *mid* tth
  _atr (omg_t,  omg)
  _atr (chi_t,  chi)
  _atr (phi_t,  phi)

  _atr (flt32,  tim)    // time
  _atr (flt32,  mon)    // monitor count
  _atr (flt32, dTim)    // delta time, may be nan
  _atr (flt32, dMon)    // delta mon. count, may be nan

  Meta(Dict::shc);
  Meta(Dict::shc, flt_vec::rc, flt32, flt32, flt32, flt32,
                               flt32, flt32, flt32, flt32);
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct (Set) SHARED   // one dataset, as acquired
  _atr (uint,      idx)   // this order in File, 1..; 0 = not in File
  _atr (Meta::sh,  meta)
  _atr (Image::sh, image)

  Set(Meta::sh, Image::sh);

  _val (tth_t::rc, tth, (), meta->tth)
  _val (omg_t::rc, omg, (), meta->omg)
  _val (phi_t::rc, phi, (), meta->phi)
  _val (chi_t::rc, chi, (), meta->chi)

  _val (flt32,     tim, (), meta->tim)
  _val (flt32,     mon, (), meta->mon)
  _val (flt32,    dTim, (), meta->dTim)
  _val (flt32,    dMon, (), meta->dMon)

  _mth (gma_rge,   rgeGma,     (Session const&))
  _mth (gma_rge,   rgeGmaFull, (Session const&))
  _mth (tth_rge,   rgeTth,     (Session const&))

  _val (inten_rge, rgeInten, (), image->rgeInten())

  _voi (collect, (Session const&, Image const* corr,
                  core::inten_vec&, core::uint_vec&, gma_rge::rc,
                  tth_t minTth, tth_t deltaTth))
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct (CombinedSet) SHARED   // one or more Set
  _atr (c::vec<Set::sh>, sets)

  CombinedSet();

  _mth (Meta::sh,  meta,  ())
  _mth (Image::sh, image, ())

  // no tth
  _mth (omg_t::rc, omg, ())
  _mth (phi_t::rc, phi, ())
  _mth (chi_t::rc, chi, ())

  _mth (flt32,     tim, ())
  _mth (flt32,     mon, ())
  _mth (flt32,    dTim, ())
  _mth (flt32,    dMon, ())

  _mth (gma_rge,   rgeGma,     (Session const&))
  _mth (gma_rge,   rgeGmaFull, (Session const&))
  _mth (tth_rge,   rgeTth,     (Session const&))

  _mth (inten_rge, rgeInten, ())

_private
  _mut (Meta::sh,  lazyMeta)
  _mut (Image::sh, lazyImage)

  _mut (omg_t,     lazyOmg)
  _mut (phi_t,     lazyPhi)
  _mut (chi_t,     lazyChi)

  _mut (flt32,     lazyTim)
  _mut (flt32,     lazyMon)
  _mut (flt32,     lazyDTim)
  _mut (flt32,     lazyDMon)

  _mth (inten_vec, collect, (Session const&, Image const* corr, gma_rge::rc))
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct (CombinedSets) SHARED
  _atr (c::vec<CombinedSet::sh>, sets)

  CombinedSets();
dcl_struct_end

//------------------------------------------------------------------------------

struct Files;

dcl_struct (File) SHARED  // one file
  _atr (c::str, name)
  _ref (Files, files) _atr (uint, idx) // this order in Files, 1..; 0 = not in Files
  _atr (c::vec<std::pair<c::str COMMA c::str>>, strs)
  _atr (c::vec<Set::sh>, sets)

  File(c::strc name, Files const&);

  _voi_mut (addSet, (Set::sh))
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct (Files) SHARED // the whole file group
  _atr (c::vec<File::sh>, files)
  _atr (Meta::Dict::sh,   dict)

  Files();

  _voi_mut (addFile, (data::File::sh))
  _voi_mut (remFile, (uint))
dcl_struct_end

//------------------------------------------------------------------------------
}}
#endif
