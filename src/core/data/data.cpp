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
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>
#include "../session.hpp"
#include "../typ/range.hpp"

namespace core { namespace data {
//------------------------------------------------------------------------------

uint Meta::Dict::enter(strc key) {
  auto it = base::find(key);
  if (base::end() != it)
    return it->second;

  uint val = size();

  EXPECT_(keys.size() == val)
  mut(keys).add(key);

  EXPECT_(checked.size() == val)
  mut(checked).add(false);

  return base::add(key, val);
}

uint Meta::Dict::at(strc key) const may_err {
  auto it = base::find(key);
  if (base::end() == it)
    l::err(str("Dict has no key: ") + key);
  return it->second;
}

Meta::Meta(Dict::shrc dict_)
: comment()
, dict(dict_), vals(dict->size(), 0), tth(0), omg(0), chi(0), phi(0)
, tim(0), mon(0) , dTim(0), dMon(0) {}

Meta::Meta(Dict::shrc dict_, flt_vec const& vals_,
           flt32 tth_, flt32 omg_, flt32 chi_,  flt32 phi_,
           flt32 tim_, flt32 mon_, flt32 dTim_, flt32 dMon_)
: comment()
, dict(dict_), vals(vals_), tth(tth_), omg(omg_), chi(chi_), phi(phi_)
, tim(tim_), mon(mon_) , dTim(dTim_), dMon(dMon_) {
  EXPECT_(dict->size() == vals.size())
}

TEST_("dict",
  Meta::Dict dict;
  CHECK_EQ(0, dict.size());

  CHECK_EQ(0, dict.enter("0"));
  CHECK_EQ(1, dict.enter("1"));
  CHECK_EQ(0, dict.enter("0"));
  CHECK_EQ(2, dict.enter("2"));

  CHECK_EQ(3, dict.size());
)

#ifndef _WIN32 // CDB has some trouble with this
TEST_("dict-throw",
  Meta::Dict dict;
  CHECK_THROWS_AS(dict.at(""), l::exc::rc);
)
#endif

//------------------------------------------------------------------------------

Set::Set(Meta::sh meta_, Image::sh image_)
: idx(0), meta(meta_), image(image_) {}

l::sz2 Set::imageSize() const {
  EXPECT_(image)
  return image->size();
}

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
                core::inten_vec& intens, uint_vec& counts,
                gma_rge::rc rgeGma, tth_t minTth, tth_t deltaTth) const {
  auto &map = *s.angleMap(*this);

  uint_vec const* gmaIndexes = nullptr;
  uint gmaIndexMin = 0, gmaIndexMax = 0;
  map.getGmaIndexes(rgeGma, gmaIndexes, gmaIndexMin, gmaIndexMax);

  EXPECT_(gmaIndexes)
  EXPECT_(gmaIndexMin <= gmaIndexMax)
  EXPECT_(gmaIndexMax <= gmaIndexes->size())

  auto size = intens.size();
  EXPECT_(size == counts.size())

  EXPECT_(0 < deltaTth)

  for (auto i = gmaIndexMin; i < gmaIndexMax; ++i) {
    auto ind   = gmaIndexes->at(i);
    auto inten = image->inten(ind);
    if (l::isnan(inten))
      continue;

    inten_t ci = corr ? corr->inten(ind) : inten_t(1);
    if (l::isnan(ci))
      continue;

    inten *= ci;

    tth_t tth  = map.at(ind).tth;

    // bin index
    auto ti = l::floor((tth - minTth) / deltaTth);
    EXPECT_(ti <= size)
    ti = l::min(l::to_sz(ti), l::to_sz(size-1)); // it can overshoot due to floating point calculation

    intens.refAt(ti) += inten;
    counts.refAt(ti) += 1;
  }
}

//------------------------------------------------------------------------------

Sets::Sets() : lazyFoldImage() {}

l::sz2 Sets::imageSize() const {
  EXPECT_(!isEmpty())
  // all images have the same size; simply take the first one
  return first()->imageSize();
}

Image::sh Sets::foldImage() const {
  if (lazyFoldImage)
    return lazyFoldImage;

  uint n = size();
  EXPECT_(0 < n)

  lazyFoldImage.reset(new Image(first()->image->size()));

  Image::ref d = mut(*lazyFoldImage);
  for_i_(n)
    d.addIntens(*(at(i)->image));

  return lazyFoldImage;
}

//------------------------------------------------------------------------------

CombinedSet::CombinedSet() : parent(nullptr)
, lazyMeta()
, lazyOmg(l::flt_nan), lazyPhi(l::flt_nan), lazyChi(l::flt_nan)
, lazyTim(l::flt_nan), lazyMon(l::flt_nan), lazyDTim(l::flt_nan), lazyDMon(l::flt_nan) {}

Meta::sh CombinedSet::meta() const {
  if (lazyMeta)
    return lazyMeta;

  // max tim, mon
  // sum dTim, dMon
  // avg all else

  uint n = size();
  EXPECT_(0 < n)

  mut(lazyMeta).reset(new Meta(first()->meta->dict));

  Meta::ref d = mut(*lazyMeta);
  for_i_(n) {
    Meta::rc s = *(at(i)->meta);

    EXPECT_(d.vals.size() == s.vals.size())
    for_i_(d.vals.size())
      mut(d.vals).refAt(i) += s.vals.at(i);

    mut(d.tth) = d.tth + s.tth;
    mut(d.omg) = d.omg + s.omg;
    mut(d.chi) = d.chi + s.chi;
    mut(d.phi) = d.phi + s.phi;

    mut(d.tim) = l::max(d.tim, s.tim);
    mut(d.mon) = l::max(d.mon, s.mon);

    mut(d.dTim) += l::isnan(s.dTim) ? 0.f : s.dTim;
    mut(d.dMon) += l::isnan(s.dMon) ? 0.f : s.dMon;
  }

  real fac = 1.0 / n;

  for_i_(d.vals.size())
    mut(d.vals).setAt(i, inten_t(d.vals.at(i) * fac));

  mut(d.tth) = d.tth * fac;
  mut(d.omg) = d.omg * fac;
  mut(d.chi) = d.chi * fac;
  mut(d.phi) = d.phi * fac;

  return lazyMeta;
}

#define AVG_SETS_VAL(lazyVal, fun)  \
  if (l::isnan(lazyVal)) {            \
    EXPECT_(!isEmpty())        \
    for (auto& set: *this)           \
      mut(lazyVal) += set->fun();   \
    mut(lazyVal) /= size();    \
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
  if (l::isnan(lazyVal)) {            \
    EXPECT_(!isEmpty())        \
    for (auto& set: *this)           \
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
  EXPECT_(!isEmpty())          \
  Range rge;                        \
  for (auto& set : *this)            \
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

inten_vec CombinedSet::collectIntens(
    Session::rc session, Image const* intensCorr, gma_rge::rc rgeGma) const
{
  auto tthRge = rgeTth(session);
  auto tthWdt = tth_t(tthRge.width());

  auto cut = session.imageCut;
  uint pixWidth = session.imageSize.i - cut.left - cut.right;

  uint numBins;
  if (1 < size()) {  // combined datasets
    auto one   = first();
    auto delta = tth_t(one->rgeTth(session).width() / pixWidth);
    numBins    = l::to_uint(l::ceil(tthWdt / delta));
  } else {
    numBins    = pixWidth; // simply match the number of pixels
  }

  inten_vec intens(numBins, inten_t(0));
  uint_vec  counts(numBins, 0);

  auto minTth   = tth_t(tthRge.min);
  auto deltaTth = tthWdt / real(numBins);

  for (auto& one : *this)
    one->collect(session, intensCorr, intens, counts, rgeGma, minTth, deltaTth);

  // sum or average
  if (session.avgScaleIntens) {
    auto scale = session.intenScale;
    for_i_(numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens.refAt(i) *= (scale / cnt);
    }
  }

  return intens;
}

inten_vec CombinedSet::collect(Session::rc s, Image const* corr, gma_rge::rc rgeGma) const {
  tth_rge tthRge = rgeTth(s);
  tth_t   tthWdt = tth_t(tthRge.width());

  auto cut = s.imageCut;
  uint pixWidth = s.imageSize.i - cut.left - cut.right;

  uint numBins;
  if (1 < size()) {
    auto one   = first();
    auto delta = tth_t(one->rgeTth(s).width() / pixWidth);
    numBins = l::to_uint(l::ceil(tthWdt / delta));
  } else {
    numBins = pixWidth; // simply match the pixel resolution
  }

  inten_vec intens(numBins, inten_t(0));
  uint_vec  counts(numBins, 0);

  auto minTth = tth_t(tthRge.min), deltaTth = tthWdt / real(numBins);

  for (auto& one : *this)
    one->collect(s, corr, intens, counts, rgeGma, minTth, deltaTth);

  // sum or average
  if (s.avgScaleIntens) {
    auto scale = s.intenScale;
    for_i_(numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens.refAt(i) *= flt32(scale/cnt);
    }
  }

  return intens;
}

//------------------------------------------------------------------------------

CombinedSets::CombinedSets() {
  resetLazies();
}

CombinedSets::ref CombinedSets::add(l::give_me<CombinedSet> set) {
  EXPECT_(!set->parent)
  mut(set->parent) = this;
  base::add(l::share(set));
  resetLazies();
  RTHIS
}

l::sz2 CombinedSets::imageSize() const {
  if (isEmpty())
    return l::sz2(0,0);

  // all images have the same size; simply take the first one
  return first()->imageSize();
}

flt32 CombinedSets::mon() const {
  AVG_SETS_VAL(lazyMon, mon)
  return lazyMon;
}

flt32 CombinedSets::dTim() const {
  AVG_SETS_VAL(lazyDTim, dTim)
  return lazyDTim;
}

flt32 CombinedSets::dMon() const {
  AVG_SETS_VAL(lazyDMon, dMon)
  return lazyDMon;
}

inten_rge::rc CombinedSets::rgeFixedInten(Session::rc session, bool trans, bool cut) const {
  if (!lazyRgeFixedInten.isDef()) {
    l_io::busy __;

    for (auto& set: *this)
      for (auto& one : *set) {
        if (one->image) {
          auto& image = *one->image;
          auto imageLens = session.imageLens(image,*this,trans,cut);
          lazyRgeFixedInten.extendBy(imageLens->rgeInten(false));
        }
      }
  }

  return lazyRgeFixedInten;
}

void CombinedSets::resetLazies() {
  lazyMon = lazyDTim = lazyDMon = l::flt_nan;
  lazyRgeFixedInten = inten_rge();
}

//------------------------------------------------------------------------------

File::File(Files::rc files_, l_io::path::rc path_)
: files(files_), isActive(true), idx(0), path(path_), name(path.filename()), comment()
, strs(), sets() {}

File::ref File::addSet(Set::sh set) {
  mut(sets).add(set);
  return *this;
}

//------------------------------------------------------------------------------

Files::Files() : dict(new Meta::Dict) {}

Files::ref Files::addFile(data::File::sh file) {
  EXPECT_(this == &file->files)
  EXPECT_(! // not there
    ([&]() {
      for_i_(size())
        if (file == at(i))
          return true;
      return false;
     }())
  )

  add(file);
  mut(file->idx) = size();
  return *this;
}

Files::ref Files::remFile(uint i) {
  File::sh file = at(i);
  rem(i);

  // renumber
  mut(file->idx) = 0;
  for_i_(size())
    mut(at(i)->idx) = i + 1;

  return *this;
}

//------------------------------------------------------------------------------

TEST_("data::sh",
  Files fs;
  File::sh f1(new File(fs, l_io::path(""))), f2(new File(fs, l_io::path("")));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST_("data",
  Files fs;

  File *f1 = new File(fs, l_io::path("")), *f2 = new File(fs, l_io::path(""));
  CHECK_EQ(0, f1->idx);
  CHECK_EQ(0, f2->idx);

  fs.addFile(l::share(f1));
  fs.addFile(l::share(f2));
  CHECK_EQ(1, f1->idx);
  CHECK_EQ(2, f2->idx);

  f1->addSet(l::share(new Set(l::share(new Meta(fs.dict, flt_vec(), 0, 0, 0, 0, 0, 0, 0, 0)),
                              l::share(new Image))));
  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof