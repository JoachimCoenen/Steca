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

#include "../typ/def.hpp"
#include "../typ/image.hpp"
#include <dev_lib/typ/map.hpp>

namespace core {

struct Session;

enum class eNorm {
  NONE,
  MONITOR, DELTA_MONITOR, DELTA_TIME, BACKGROUND,
};

namespace data {
//------------------------------------------------------------------------------

using flt_vec = l::vec<flt32>;

dcl_(Meta) SHARED // metadata
  // attribute dictionary
  dcl_sub_(Dict, l::map<str COMMA uint>) SHARED
    mth_mut_(uint, add, (strc));
    mth_(uint, at,  (strc)) may_err;
  };

  atr_(str, comment);
  atr_(str, date);

  atr_(Dict::sh, dict); // other than the values stored explicitly below
  atr_(flt_vec,  vals);

  atr_(tth_t,  tth);    // *mid* tth
  atr_(omg_t,  omg);
  atr_(chi_t,  chi);
  atr_(phi_t,  phi);

  atr_(flt32,  tim);   // time
  atr_(flt32,  mon);   // monitor count
  atr_(flt32, dTim);   // delta time, may be nan
  atr_(flt32, dMon);   // delta mon. count, may be nan

  Meta(Dict::shrc);
  Meta(Dict::shrc, flt_vec const&, flt32, flt32, flt32, flt32,
                                   flt32, flt32, flt32, flt32);
dcl_end

//------------------------------------------------------------------------------

dcl_(Set) SHARED   // one dataset, as acquired
  atr_(uint,      idx);  // this order in File, 1..; 0 = not in File
  atr_(Meta::sh,  meta);
  atr_(Image::sh, image);

  Set(Meta::sh, Image::sh);

  mth_(l::sz2, imageSize, ());

  mth_(tth_t::rc, tth, ()) VAL_(meta->tth)
  mth_(omg_t::rc, omg, ()) VAL_(meta->omg)
  mth_(phi_t::rc, phi, ()) VAL_(meta->phi)
  mth_(chi_t::rc, chi, ()) VAL_(meta->chi)

  mth_(flt32,     tim, ()) VAL_(meta->tim)
  mth_(flt32,     mon, ()) VAL_(meta->mon)
  mth_(flt32,    dTim, ()) VAL_(meta->dTim)
  mth_(flt32,    dMon, ()) VAL_(meta->dMon)

  mth_(gma_rge,   rgeGma,     (Session const&));
  mth_(gma_rge,   rgeGmaFull, (Session const&));
  mth_(tth_rge,   rgeTth,     (Session const&));

  mth_(inten_rge, rgeInten, ()) VAL_(image->rgeInten())

  mth_(void, collect, (Session const&, Image const* corr,
                       core::inten_vec&, core::uint_vec&, gma_rge::rc,
                       tth_t minTth, tth_t deltaTth));
dcl_end

//------------------------------------------------------------------------------

struct CombinedSets;

dcl_(CombinedSet) SHARED   // one or more Set
  atr_(l::vec<Set::sh>, sets);
  ptr_(CombinedSets, parent);

  CombinedSet();

  mth_(l::sz2, imageSize, ());

  mth_(Meta::sh,  meta,  ());
  mth_(Image::sh, image, ());

  // no tth
  mth_(omg_t::rc, omg, ());
  mth_(phi_t::rc, phi, ());
  mth_(chi_t::rc, chi, ());

  mth_(flt32,     tim, ());
  mth_(flt32,     mon, ());
  mth_(flt32,    dTim, ());
  mth_(flt32,    dMon, ());

  mth_(gma_rge,   rgeGma,     (Session const&));
  mth_(gma_rge,   rgeGmaFull, (Session const&));
  mth_(tth_rge,   rgeTth,     (Session const&));

  mth_(inten_rge, rgeInten, ());
  mth_(inten_vec, collectIntens, (Session const&,
                                  Image const* intensCorr, gma_rge::rc));

private:
  mut_(Meta::sh,  lazyMeta);
  mut_(Image::sh, lazyImage);

  mut_(omg_t,     lazyOmg);
  mut_(phi_t,     lazyPhi);
  mut_(chi_t,     lazyChi);

  mut_(flt32,     lazyTim);
  mut_(flt32,     lazyMon);
  mut_(flt32,     lazyDTim);
  mut_(flt32,     lazyDMon);

  mth_(inten_vec, collect, (Session const&, Image const* corr, gma_rge::rc));
dcl_end

//------------------------------------------------------------------------------

dcl_(CombinedSets) SHARED
  atr_(l::vec<CombinedSet::sh>, sets);

  CombinedSets();

  mth_(l::sz2, imageSize, ());

  mth_(flt32,  mon, ());
  mth_(flt32, dTim, ());
  mth_(flt32, dMon, ());

  mth_(inten_rge::rc, rgeFixedInten, (Session const&, bool trans, bool cut));

private:
  mut_(flt32, lazyMon);
  mut_(flt32, lazyDTim);
  mut_(flt32, lazyDMon);

  mutable inten_rge lazyRgeFixedInten;
dcl_end

//------------------------------------------------------------------------------

struct Files;

dcl_(File) SHARED  // one file
  ref_(Files, files);
  atr_(uint, idx); // this order in Files, 1..; 0 = not in Files
  atr_(str, name);
  atr_(str, comment);
  atr_(l::vec<std::pair<str COMMA str>>, strs);
  atr_(l::vec<Set::sh>, sets);

  File(Files const&, strc name);

  set_(addSet, (Set::sh));
dcl_end

//------------------------------------------------------------------------------

dcl_(Files) SHARED // the whole file group
  atr_(l::vec<File::sh>, files);
  atr_(Meta::Dict::sh,   dict);

  Files();

  set_(addFile, (data::File::sh));
  set_(remFile, (uint));
dcl_end

//------------------------------------------------------------------------------
}}