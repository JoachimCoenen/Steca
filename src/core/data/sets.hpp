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
#include <lib/dev/inc/vecs.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/bag.hpp>
#include <lib/dev/typ/hash.hpp>
#include <lib/dev/typ/map.hpp>
#include <lib/dev/typ/set.hpp>

namespace core {

struct Session;

namespace data {
//------------------------------------------------------------------------------
// attribute dictionaries

dcl_(MetaDictBase)
  using idx = uint;

  atr_(str_vec, keys);

  MetaDictBase();
  virtual ~MetaDictBase();

  virtual voi_mut_(clear, ());

  mth_(sz_t, size, ())      RET_(sz_t(keys.size()))
  mth_(strc, key,  (idx i)) RET_(keys.at(i))

protected:
  l::set<str> keySet;
  voi_mut_(enter, (strc key));
  voi_mut_(enter, (str_vec::rc keys));
  MetaDictBase(rc);
dcl_end

dcl_sub_(KeyBag, l::bag<str>) SHARED
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(FilesMetaDict, MetaDictBase) SHARED CLONED
  USING_BASE_(enter)
  FilesMetaDict();
private:
  FilesMetaDict(rc);
dcl_end

dcl_sub_(MetaDict, MetaDictBase) SHARED
  atr_(l::hash<str COMMA idx>, idxs);

  MetaDict();

  voi_mut_(clear, ());
  voi_mut_(enter, (strc key));
  voi_mut_(enter, (str_vec::rc keys));

  mth_mut_(idx, idxEnter, (strc key));
  mth_(idx, index,     (strc key)) RET_(idxs.at(key))
  mth_(int, safeIndex, (strc key));

private:
  MetaDict(rc);
dcl_end

//------------------------------------------------------------------------------

// attribute values
dcl_reimpl_(MetaVals, l::hash<MetaDictBase::idx COMMA flt32>)
  USING_BASE_(begin, end, clear, isEmpty)

  using idx = MetaDictBase::idx;

  mth_(flt32, valAt, (idx)) may_err;
  set_(setAt, (idx, flt32));
  set_(addAt, (idx, flt32));
dcl_end

//------------------------------------------------------------------------------

dcl_(Meta) SHARED // metadata
  atr_(str, comment);
  atr_(str, date);

  atr_(MetaDict::sh, dict); // other than the values stored explicitly below
  atr_(MetaVals,     vals); // their values

  atr_(tth_t,  tth);    // *mid* tth
  atr_(omg_t,  omg);
  atr_(chi_t,  chi);
  atr_(phi_t,  phi);

  atr_(flt32,  tim);   // time
  atr_(flt32,  mon);   // monitor count
  atr_(flt32, dTim);   // delta time, may be nan
  atr_(flt32, dMon);   // delta mon. count, may be nan

  Meta(MetaDict::sh);
  Meta(MetaDict::sh, MetaVals::rc, flt32, flt32, flt32, flt32,
                                   flt32, flt32, flt32, flt32);
dcl_end

//------------------------------------------------------------------------------

dcl_(FileSrc) SHARED
  atr_(l_io::path, path);
  atr_(str, comment);

  FileSrc(l_io::path::rc path, strc comment);
dcl_end

//------------------------------------------------------------------------------

dcl_(Set) SHARED   // one dataset, as acquired
  atr_(FileSrc::sh, src);
  atr_(Meta::sh,    meta);
  atr_(Image::sh,   image);

  Set(FileSrc::sh, Meta::sh, Image::sh);

  mth_(l::sz2, imageSize, ());

  mth_(tth_t::rc, tth, ()) RET_(meta->tth)
  mth_(omg_t::rc, omg, ()) RET_(meta->omg)
  mth_(phi_t::rc, phi, ()) RET_(meta->phi)
  mth_(chi_t::rc, chi, ()) RET_(meta->chi)

  mth_(flt32,     tim, ()) RET_(meta->tim)
  mth_(flt32,     mon, ()) RET_(meta->mon)
  mth_(flt32,    dTim, ()) RET_(meta->dTim)
  mth_(flt32,    dMon, ()) RET_(meta->dMon)

  mth_(gma_rge,   rgeGma,     (Session const&));
  mth_(gma_rge,   rgeGmaFull, (Session const&));
  mth_(tth_rge,   rgeTth,     (Session const&));

  mth_(inten_rge, rgeInten, ()) RET_(image->rgeInten())

  voi_(collect, (Session const&, Image const* corr,
                 core::inten_vec&, uint_vec&, gma_rge::rc,
                 tth_t minTth, tth_t deltaTth));
dcl_end

//------------------------------------------------------------------------------

dcl_reimpl_(Sets, l::vec<Set::sh>)
  USING_BASE_(first, begin, end, isEmpty, size)

  Sets();

  mth_(l::sz2,    imageSize, ());
  mth_(Image::sh, foldImage, ());
  set_(add, (Set::sh)) may_err;

private:
  mutable Image::sh lazyFoldImage;
dcl_end

//------------------------------------------------------------------------------

struct CombinedSets;

dcl_sub_(CombinedSet, Sets) SHARED   // one or more Set
  atr_(uint, fileNo);
  atr_(str,  tag);
  atr_(bool, isActive); // included in calculations

  CombinedSet(uint fileNo);

  mth_(Meta::sh,  meta, (core::data::FilesMetaDict::sh));

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
  mutable Meta::sh lazyMeta;

  mutable omg_t    lazyOmg;
  mutable phi_t    lazyPhi;
  mutable chi_t    lazyChi;

  mutable flt32    lazyTim;
  mutable flt32    lazyMon;
  mutable flt32    lazyDTim;
  mutable flt32    lazyDMon;

  mth_(inten_vec, collect, (Session const&, Image const* corr, gma_rge::rc));
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(CombinedSets, l::vec<CombinedSet::sh>) SHARED
  CombinedSets();

  set_(add, (l::give_me<CombinedSet>));

  mth_(l::sz2, imageSize, ());

  mth_(flt32,  mon, ());
  mth_(flt32, dTim, ());
  mth_(flt32, dMon, ());

  mth_(inten_rge::rc, rgeFixedInten, (Session const&, bool trans, bool cut));

private:
  mutable flt32 lazyMon;
  mutable flt32 lazyDTim;
  mutable flt32 lazyDMon;

  mutable inten_rge lazyRgeFixedInten;

  void resetLazies();
dcl_end

//------------------------------------------------------------------------------
}}
