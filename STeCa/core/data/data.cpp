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

#include "data.hpp"
#include "../session.hpp"
#include <c2/inc/c_cpp>

namespace core { namespace data {
//------------------------------------------------------------------------------

uint Meta::Dict::add(c::strc key) {
  try {
    return base::at(key);
  } catch (std::exception const&) {
    return base::add(key, size());
  }
}

uint Meta::Dict::at(c::strc key) const may_err {
  try {
    return base::at(key);
  } catch (std::exception const&) {
    c::err("Dict has no ", key);
  }
}

Meta::Meta(Dict::shc dict_)
: dict(dict_), vals(dict->size(), 0), tth(0), omg(0), chi(0), phi(0)
, tim(0), mon(0) , dTim(0), dMon(0) {}

Meta::Meta(Dict::shc dict_, flt_vec::rc vals_,
           flt32 tth_, flt32 omg_, flt32 chi_,  flt32 phi_,
           flt32 tim_, flt32 mon_, flt32 dTim_, flt32 dMon_)
  : dict(dict_), vals(vals_), tth(tth_), omg(omg_), chi(chi_), phi(phi_)
  , tim(tim_), mon(mon_) , dTim(dTim_), dMon(dMon_) {
  EXPECT (dict->size() == vals.size())
}

TEST("dict",
  Meta::Dict dict;
  CHECK_EQ(0, dict.size());

  CHECK_EQ(0, dict.add("0"));
  CHECK_EQ(1, dict.add("1"));
  CHECK_EQ(0, dict.add("0"));
  CHECK_EQ(2, dict.add("2"));

  CHECK_EQ(3, dict.size());
)

#ifndef OS_WIN  // CDB has some trouble with this
TEST("dict-throw",
  Meta::Dict dict;
  CHECK_THROWS_AS(dict.at(""), c::exc::rc);
)
#endif

//------------------------------------------------------------------------------

Set::Set(Meta::sh meta_, Image::sh image_)
: idx(0), meta(meta_), image(image_) {}

gma_rge Set::rgeGma(Session::rc s) const {
  return s.angleMap(*this)->rgeGma;
}

gma_rge Set::rgeGmaFull(Session::rc s) const {
  return s.angleMap(*this)->rgeGmaFull;
}

tth_rge Set::rgeTth(Session::rc s)const {
  return s.angleMap(*this)->rgeTth;
}

void Set::collect(Session::rc s, Image const* corr,
                core::inten_vec& intens, core::uint_vec& counts,
                gma_rge::rc rgeGma, tth_t minTth, tth_t deltaTth) const {
  auto &map = *s.angleMap(*this);

  uint_vec const* gmaIndexes = nullptr;
  uint gmaIndexMin = 0, gmaIndexMax = 0;
  map.getGmaIndexes(rgeGma, gmaIndexes, gmaIndexMin, gmaIndexMax);

  EXPECT (gmaIndexes)
  EXPECT (gmaIndexMin <= gmaIndexMax)
  EXPECT (gmaIndexMax <= gmaIndexes->size())

  auto size = intens.size();
  EXPECT (size == counts.size())

  EXPECT (0 < deltaTth)

  for (auto i = gmaIndexMin; i < gmaIndexMax; ++i) {
    auto ind   = gmaIndexes->at(i);
    auto inten = image->inten(ind);
    if (c::isnan(inten))
      continue;

    inten_t ci = corr ? corr->inten(ind) : 1;
    if (c::isnan(ci))
      continue;

    inten *= ci;

    tth_t tth  = map.at(ind).tth;

    // bin index
    auto ti = c::to_uint(c::floor((tth - minTth) / deltaTth));
    EXPECT (ti <= size)
    ti = c::min(ti, size-1); // it can overshoot due to floating point calculation

    intens.refAt(ti) += inten;
    counts.refAt(ti) += 1;
  }
}

//------------------------------------------------------------------------------

CombinedSet::CombinedSet() : sets()
, lazyOmg(c::flt_nan), lazyPhi(c::flt_nan), lazyChi(c::flt_nan)
, lazyTim(c::flt_nan), lazyMon(c::flt_nan), lazyDTim(c::flt_nan), lazyDMon(c::flt_nan) {}

Meta::sh CombinedSet::meta() const {
  if (lazyMeta.ptr())
    return lazyMeta;

  // max tim, mon
  // sum dTim, dMon
  // avg all else

  uint n = sets.size();
  EXPECT (0 < n)

  mut(lazyMeta).reset(new Meta(sets.first()->meta->dict));

  Meta::ref d = mut(*lazyMeta);
  for_i (n) {
    Meta::rc s = *(sets.at(i)->meta);

    EXPECT (d.vals.size() == s.vals.size())
    for_i (d.vals.size())
      mut(d.vals).setAt(i, d.vals.at(i) + s.vals.at(i));

    mut(d.tth) = d.tth + s.tth;
    mut(d.omg) = d.omg + s.omg;
    mut(d.chi) = d.chi + s.chi;
    mut(d.phi) = d.phi + s.phi;

    mut(d.tim) = c::max(d.tim, s.tim);
    mut(d.mon) = c::max(d.mon, s.mon);

    mut(d.dTim) += c::notnan(s.dTim, 0.f); // TODO consider _use_num !nan etc.
    mut(d.dMon) += c::notnan(s.dMon, 0.f);
  }

  real fac = 1.0 / n;

  for_i (d.vals.size())
    mut(d.vals).setAt(i, d.vals.at(i) * fac);

  mut(d.tth) = d.tth * fac;
  mut(d.omg) = d.omg * fac;
  mut(d.chi) = d.chi * fac;
  mut(d.phi) = d.phi * fac;

  return lazyMeta;
}

Image::sh CombinedSet::image() const {
  if (lazyImage.ptr())
    return lazyImage;

  uint n = sets.size();
  EXPECT (0 < n)

  mut(lazyImage).reset(new Image(sets.first()->image->size()));

  Image::ref d = mut(*lazyImage);
  for_i (n)
    d.addIntens(*(sets.at(i)->image));

  return lazyImage;
}

#define AVG_SETS_VAL(lazyVal, fun)  \
  if (c::isnan(lazyVal)) {          \
    EXPECT (!sets.isEmpty())        \
    for (auto& set: sets)           \
      mut(lazyVal) += set->fun();   \
    mut(lazyVal) /= sets.size();    \
  }

omg_t::rc CombinedSet::omg() const {
  AVG_SETS_VAL(lazyOmg.val, omg)
  return lazyOmg;
}

phi_t::rc CombinedSet::phi() const {
  AVG_SETS_VAL(lazyPhi.val, phi)
  return lazyPhi;
}

chi_t::rc CombinedSet::chi() const {
  AVG_SETS_VAL(lazyChi.val, chi)
  return lazyChi;
}

flt32 CombinedSet::tim() const {
  AVG_SETS_VAL(lazyTim, tim)
  return lazyTim;
}

flt32 CombinedSet::mon() const {
  AVG_SETS_VAL(lazyMon, mon)
  return lazyMon;
}

#define SUM_SETS_VAL(lazyVal, fun)  \
  if (c::isnan(lazyVal)) {          \
    EXPECT (!sets.isEmpty())        \
    for (auto& set: sets)           \
      mut(lazyVal) += set->fun();   \
  }

flt32 CombinedSet::dTim() const {
  SUM_SETS_VAL(lazyDTim, dTim)
  return lazyDTim;
}

flt32 CombinedSet::dMon() const {
  SUM_SETS_VAL(lazyDMon, dMon)
  return lazyDMon;
}

#define RGE_SETS_COMBINE(op, fun)   \
  EXPECT (!sets.isEmpty())          \
  Range rge;                        \
  for (auto& set : sets)            \
    rge.op(set->fun);               \
  return rge;

gma_rge CombinedSet::rgeGma(Session const& s) const {
  RGE_SETS_COMBINE(extendBy, rgeGma(s))
}

gma_rge CombinedSet::rgeGmaFull(Session const& s) const {
  RGE_SETS_COMBINE(extendBy, rgeGmaFull(s))
}

tth_rge CombinedSet::rgeTth(Session const& s) const {
  RGE_SETS_COMBINE(extendBy, rgeTth(s))
}

inten_rge CombinedSet::rgeInten() const {
  RGE_SETS_COMBINE(intersect, rgeInten())
}

inten_vec CombinedSet::collect(Session::rc s, Image const* corr, gma_rge::rc rgeGma) const {
  tth_rge tthRge = rgeTth(s);
  tth_t   tthWdt = tth_t(tthRge.width());

  auto cut = s.imageCut;
  uint pixWidth = s.imageSize.i - cut.left - cut.right;

  uint numBins;
  if (1 < sets.size()) {
    auto one   = sets.first();
    auto delta = tth_t(one->rgeTth(s).width() / pixWidth);
    numBins = c::to_uint(c::ceil(tthWdt / delta));
  } else {
    numBins = pixWidth; // simply match the pixel resolution
  }

  inten_vec intens(numBins, 0);
  uint_vec  counts(numBins, 0);

  auto minTth = tth_t(tthRge.min), deltaTth = tth_t(tthWdt / numBins);

  for (auto& one : sets)
    one->collect(s, corr, intens, counts, rgeGma, minTth, deltaTth);

  // sum or average
  if (s.avgScaleIntens) {
    auto scale = s.intenScale;
    for_i (numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens.refAt(i) *= scale/cnt;
    }
  }

  return intens;
}

//------------------------------------------------------------------------------

File::File(c::strc name_, Files::rc files_)
: name(name_), files(files_), idx(0), strs(), sets() {}

void File::addSet(Set::sh set) {
  mut(sets).add(set);
}

//------------------------------------------------------------------------------

Files::Files() : files(), dict(new Meta::Dict) {}

void Files::addFile(data::File::sh file) {
  EXPECT (this == &file->files)
  EXPECT (! // not there
    ([&]() {
      for_i (files.size())
        if (file == files.at(i))
          return true;
      return false;
     }())
  )

  mut(files).add(file);
  mut(file->idx) = files.size();
}

void Files::remFile(uint i) {
  File::sh file = files.at(i);
  mut(files).rem(i);

  // renumber
  mut(file->idx) = 0;
  for_i (files.size())
    mut(files.at(i)->idx) = i + 1;
}

//------------------------------------------------------------------------------

TEST("data::sh",
  Files fs;
  File::sh f1(new File("", fs)), f2(new File("", fs));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST("data",
  Files fs;

  File *f1 = new File("", fs), *f2 = new File("", fs);
  CHECK_EQ(0, f1->idx);
  CHECK_EQ(0, f2->idx);

  fs.addFile(c::share(f1));
  fs.addFile(c::share(f2));
  CHECK_EQ(1, f1->idx);
  CHECK_EQ(2, f2->idx);

  f1->addSet(c::share(new Set(c::share(new Meta(fs.dict, flt_vec(), 0, 0, 0, 0, 0, 0, 0, 0)),
                              c::share(new Image))));
  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof
