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
#include "../typ/image.hpp"
#include <lib/dev/inc/vecs.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/hash.hpp>
#include <lib/dev/typ/map.hpp>
#include <lib/dev/typ/set.hpp>
#include <lib/dev/typ/uset.hpp>

namespace core { namespace data {
//------------------------------------------------------------------------------

/** a dictionary of metadata attributes
 */
dcl_(MetaDictBase)
  using idx = uint;

  /// current set of keys, ordered
  atr_(str_vec, keys);

  MetaDictBase();
  virtual ~MetaDictBase();

  virtual mut_(clear, ());

  mth_(sz_t, size, ())      RET_(sz_t(keys.size()))
  mth_(strc, key,  (idx i)) RET_(keys.at(i))

protected:
  /// current set of keys, fast to search
  l::set<str> keySet;
  mut_(enter, (strc key));
  mut_(enter, (str_vec::rc keys));
  MetaDictBase(rc);
dcl_end

/// A bag of keys
dcl_sub_(KeyBag, l::uset<str>) SHARED
dcl_end

//------------------------------------------------------------------------------

/** metadata dictionary for Files */
dcl_sub_(FilesMetaDict, MetaDictBase) SHARED CLONED
  UB1_(enter)
  FilesMetaDict();
private:
  FilesMetaDict(rc);
dcl_end

/** metadata dictionary */
dcl_sub_(MetaDict, MetaDictBase) SHARED
  atr_(l::hash<str COMMA idx>, idxs);

  MetaDict();

  mut_(clear, ());
  mut_(enter, (strc key));
  mut_(enter, (str_vec::rc keys));

  mth_mut_(idx, idxEnter, (strc key));
  mth_(idx, index,     (strc key)) RET_(idxs.at(key))
  mth_(int, safeIndex, (strc key));

private:
  MetaDict(rc);
dcl_end

//------------------------------------------------------------------------------

/** A storage for metadata values */
dcl_reimpl_(MetaVals, l::hash<MetaDictBase::idx COMMA flt32>)
  UB4_(begin, end, clear, isEmpty)

  using idx = MetaDictBase::idx;

  mth_(flt32, valAt, (idx)) may_err;
  set_(setAt, (idx, flt32));
  set_(addAt, (idx, flt32));
dcl_end

//------------------------------------------------------------------------------

/** Metadata: several explicitely stored values; those and a number of others
 * are also in @c dict and @c vals */
dcl_(Meta) SHARED
  atr_(str,    comment);
  atr_(str,    date);

  atr_(tth_t,  tth);  ///< "middle" 2theta
  atr_(omg_t,  omg);  ///< cradle
  atr_(chi_t,  chi);  ///< cradle
  atr_(phi_t,  phi);  ///< cradle

  atr_(flt32,  tim);  ///< time
  atr_(flt32,  mon);  ///< monitor count
  atr_(flt32, dTim);  ///< delta time, may be NaN
  atr_(flt32, dMon);  ///< delta monitor count, may be NaN

  /// dynamic dictionary of metadata values
  atr_(MetaDict::shp, dict);
  /// values of metadata in the above dictionary
  atr_(MetaVals,      vals);

  Meta(MetaDict::shp);
  Meta(MetaDict::shp, MetaVals::rc, flt32, flt32, flt32, flt32,
                                   flt32, flt32, flt32, flt32);
dcl_end

//------------------------------------------------------------------------------

/// Information on source (disk) file.
dcl_(FileSrc) SHARED
  atr_(l_io::path, path);
  atr_(str, comment);

  FileSrc(l_io::path::rc path, strc comment);
dcl_end

//------------------------------------------------------------------------------

/// One dataset, as read from the source file
dcl_(Set) SHARED
  atr_(FileSrc::shp, src);    ///< source file information
  atr_(Meta::shp,    meta);   ///< metadata
  atr_(Image::shp,   image);  ///< detector image

  Set(FileSrc::shp, Meta::shp, Image::shp);

  mth_(l::sz2, imageSize, ());

  mth_(tth_t::rc, tth, ()) RET_(meta->tth)
  mth_(omg_t::rc, omg, ()) RET_(meta->omg)
  mth_(phi_t::rc, phi, ()) RET_(meta->phi)
  mth_(chi_t::rc, chi, ()) RET_(meta->chi)

  mth_(flt32,     tim, ()) RET_(meta->tim)
  mth_(flt32,     mon, ()) RET_(meta->mon)
  mth_(flt32,    dTim, ()) RET_(meta->dTim)
  mth_(flt32,    dMon, ()) RET_(meta->dMon)

  mth_(gma_rge,   rgeGma,     (calc::FitParams const&));
  mth_(gma_rge,   rgeGmaFull, (calc::FitParams const&));
  mth_(tth_rge,   rgeTth,     (calc::FitParams const&));

  mth_(inten_rge, rgeInten, ()) RET_(image->rgeInten())

  /// collect pixels (intensities) that are within a 2theta and gamma ranges
  voi_(collect, (calc::FitParams const&,
                 core::inten_vec&, uint_vec&, gma_rge::rc,
                 tth_t minTth, tth_t deltaTth));
dcl_end

//------------------------------------------------------------------------------

/// A collection of datasets (base class)
dcl_reimpl_(Sets, l::vec<Set::shr>)
  UB5_(first, begin, end, isEmpty, size)

  Sets();

  mth_(l::sz2,    imageSize, ());
  mth_(Image::shp, foldImage, ());
  set_(add, (Set::shr)) may_err;

private:
  mutable Image::shp lazyFoldImage;
dcl_end

//------------------------------------------------------------------------------

struct CombinedSets;

/** A "combined set" - a collection of one or more datasets, that is seen as
 * a single dataset. Its metadata are calculated from individual datasets:
 * the maximum of time + monitor count; the sum of delta time and delta monitor
 * count; the average of everything else.
 */
dcl_sub_(CombinedSet, Sets) SHARED
  /// the number of the file from which (the first of) the datasets came
  atr_(uint, fileNo);
  /// tag to display (a range of original set numbers)
  atr_(str,  tag);
  /// is it included in calculations ?
  atr_(bool, isActive);

  CombinedSet(uint fileNo);

  mth_(Meta::shp,  meta, ());

  // no tth
  mth_(omg_t::rc, omg, ());
  mth_(phi_t::rc, phi, ());
  mth_(chi_t::rc, chi, ());

  mth_(flt32,     tim, ());
  mth_(flt32,     mon, ());
  mth_(flt32,    dTim, ());
  mth_(flt32,    dMon, ());

  mth_(gma_rge,   rgeGma,     (calc::FitParams const&));
  mth_(gma_rge,   rgeGmaFull, (calc::FitParams const&));
  mth_(tth_rge,   rgeTth,     (calc::FitParams const&));

  mth_(inten_rge, rgeInten, ());
  mth_(inten_vec, collectIntens, (calc::FitParams const&, gma_rge::rc));

private:
  mutable Meta::shp lazyMeta;

  mutable omg_t    lazyOmg;
  mutable phi_t    lazyPhi;
  mutable chi_t    lazyChi;

  mutable flt32    lazyTim;
  mutable flt32    lazyMon;
  mutable flt32    lazyDTim;
  mutable flt32    lazyDMon;

  /// calculate the histogram
  mth_(inten_vec, histogram, (calc::FitParams const&, gma_rge::rc));
dcl_end

//------------------------------------------------------------------------------

/** The complete set of datasets in an experiment is a collection of
 * combined sets
 */
dcl_sub_(CombinedSets, l::vec<CombinedSet::shr>) SHARED
  CombinedSets();

  set_(add, (l::give_me<CombinedSet>));

  mth_(l::sz2, imageSize, ());

  mth_(flt32,  mon, ());
  mth_(flt32, dTim, ());
  mth_(flt32, dMon, ());

  mth_(inten_rge::rc, rgeFixedInten, (calc::FitParams const&, bool trans, bool cut));
  mth_(core::data::CombinedSet::shr, combineAll, ());

private:
  mutable flt32 lazyMon;
  mutable flt32 lazyDTim;
  mutable flt32 lazyDMon;

  mutable inten_rge lazyRgeFixedInten;

  void resetLazies();
dcl_end

//------------------------------------------------------------------------------
}}
// eof
