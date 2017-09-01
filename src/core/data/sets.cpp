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

#include "sets.hpp"
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>
#include "../session.hpp"

namespace core { namespace data {
//------------------------------------------------------------------------------

MetaDict::MetaDict() : idxs() {}

uint MetaDict::enter(strc key) {
  auto it = idxs.find(key);
  if (idxs.end() != it)
    return it->second;

  uint idx = idxs.size();
  mut(idxs).add(key, idx);
  mut(keys).add(key);
  ENSURE_(idxs.size() == keys.size())

  return idx;
}

int MetaDict::safeIndex(strc key) const {
  return idxs.contains(key) ? int(idxs.at(key)) : -1;
}

//------------------------------------------------------------------------------

FilesMetaDict::FilesMetaDict() : checks() {}

uint FilesMetaDict::enter(strc key) {
  if (!checks.contains(key))
    mut(checks).add(key, false);

  uint idx = base::enter(key);
  ENSURE_(checks.size() == base::size())
  return idx;
}

FilesMetaDict::ref FilesMetaDict::update(l::set<str>::rc keys) {
  for (auto it = checks.begin(); it != checks.end(); ++it)
    if (!keys.contains(it->first))
      mut(checks).erase(it);

  for (auto&& key : keys)
    enter(key);

  RTHIS
}

bool FilesMetaDict::checked(strc key) const may_err {
  return checks.at(key);
}

FilesMetaDict::ref FilesMetaDict::check(strc key, bool on) may_err {
  mut(checks).at(key) = on;
  RTHIS
}

//------------------------------------------------------------------------------

flt32 MetaVals::valAt(uint i) const may_err {
  return at(i);
}

MetaVals::ref MetaVals::setAt(uint i, flt32 val) {
  if (contains(i))
    at(i) = val;
  else
    add(i, val);
  RTHIS
}

MetaVals::ref MetaVals::addAt(uint i, flt32 val) {
  if (contains(i))
    val += at(i);
  setAt(i, val);
  RTHIS
}

//------------------------------------------------------------------------------

Meta::Meta(MetaDict::sh dict_)
: comment()
, dict(dict_), vals(), tth(0.), omg(0.), chi(0.), phi(0.)
, tim(0), mon(0) , dTim(0), dMon(0) {}

Meta::Meta(MetaDict::sh dict_, MetaVals::rc vals_,
           flt32 tth_, flt32 omg_, flt32 chi_,  flt32 phi_,
           flt32 tim_, flt32 mon_, flt32 dTim_, flt32 dMon_)
: comment()
, dict(dict_), vals(vals_), tth(tth_), omg(omg_), chi(chi_), phi(phi_)
, tim(tim_), mon(mon_) , dTim(dTim_), dMon(dMon_) {
}

TEST_("dict",
  MetaDict dict;
  CHECK_EQ(0, dict.enter("0"));
  CHECK_EQ(1, dict.enter("1"));
  CHECK_EQ(0, dict.enter("0"));
  CHECK_EQ(2, dict.enter("2"));
)

#ifndef _WIN32 // CDB has some trouble with this
TEST_("dict-throw",
  MetaDict dict;
  CHECK_THROWS_AS(dict.index(""), l::exc::rc);
)
#endif

//------------------------------------------------------------------------------

FileIdx::FileIdx(uint val_) : val(val_) {}

//------------------------------------------------------------------------------

Set::Set(FileIdx::sh idx_, Meta::sh meta_, Image::sh image_)
: idx(idx_), meta(meta_), image(image_) {}

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
  auto&& map = *s.angleMap(*this);

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
    auto ti = l::to_uint(l::floor((tth - minTth) / deltaTth));
    EXPECT_(ti <= size)
    ti = l::min(ti, size-1); // it can overshoot due to floating point calculation

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

  lazyFoldImage.reset(new Image(imageSize()));

  Image::ref d = mut(*lazyFoldImage);
  for_i_(n)
    d.addIntens(*(at(i)->image));

  return lazyFoldImage;
}

Sets::ref Sets::add(Set::sh set) may_err {
  check_or_err_(isEmpty() || first()->imageSize() == set->imageSize(),
                "Inconsistent image size");
  base::add(set);
  RTHIS;
}

//------------------------------------------------------------------------------

CombinedSet::CombinedSet()
: isActive(true)
, lazyMeta()
, lazyOmg(l::flt64_nan), lazyPhi(l::flt64_nan), lazyChi(l::flt64_nan)
, lazyTim(l::flt32_nan), lazyMon(l::flt32_nan), lazyDTim(l::flt32_nan), lazyDMon(l::flt32_nan) {}

Meta::sh CombinedSet::meta(core::data::FilesMetaDict::sh fullDict) const {
  if (lazyMeta)
    return lazyMeta;

  uint n = size();
  EXPECT_(0 < n)

  if (1==n)
    return (lazyMeta = first()->meta);

  lazyMeta.reset(new Meta(*reinterpret_cast<core::data::MetaDict::sh*>(&fullDict)));

  Meta::ref lm = mut(*lazyMeta);
  MetaDict::rc ld = *lm.dict;
  MetaVals::rc lv = lm.vals;

  // max tim, mon
  // sum dTim, dMon
  // avg all else

  for_i_(n) {
    Meta::rc im = *(at(i)->meta);

    mut(lm.tth) = lm.tth + im.tth;
    mut(lm.omg) = lm.omg + im.omg;
    mut(lm.chi) = lm.chi + im.chi;
    mut(lm.phi) = lm.phi + im.phi;

    mut(lm.tim) = l::max(lm.tim, im.tim);
    mut(lm.mon) = l::max(lm.mon, im.mon);

    mut(lm.dTim) += l::isnan(im.dTim) ? 0.f : im.dTim; // TODO handle nans properly
    mut(lm.dMon) += l::isnan(im.dMon) ? 0.f : im.dMon;

    MetaDict::rc id = *im.dict;
    MetaVals::rc iv = im.vals;

    for (auto&& k : id.keys)
      mut(lv).addAt(ld.index(k), iv.valAt(id.index(k)));
  }

  real fac = 1.0 / n;

  mut(lm.tth) = lm.tth * fac;
  mut(lm.omg) = lm.omg * fac;
  mut(lm.chi) = lm.chi * fac;
  mut(lm.phi) = lm.phi * fac;

  for (auto&& v : lv)
    mut(v.second) *= fac;

  return lazyMeta;
}

#define AVG_SETS_VAL(lazyVal, fun)  \
  if (l::isnan(lazyVal)) {            \
    EXPECT_(!isEmpty())        \
    for (auto&& set: *this)           \
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
    for (auto&& set: *this)           \
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
  for (auto&& set : *this)            \
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

  for (auto&& one : *this)
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

  for (auto&& one : *this)
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
  base::add(l::sh(set));
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

    for (auto&& set: *this)
      for (auto&& one : *set) {
        if (one->image) {
          auto&& image = *one->image;
          auto imageLens = session.imageLens(image,*this,trans,cut);
          lazyRgeFixedInten.extendBy(imageLens->rgeInten(false));
        }
      }
  }

  return lazyRgeFixedInten;
}

void CombinedSets::resetLazies() {
  lazyMon = lazyDTim = lazyDMon = l::flt32_nan;
  lazyRgeFixedInten = inten_rge();
}

//------------------------------------------------------------------------------

TEST_("data::sh",
  File::sh f1(new File(l_io::path(""))), f2(new File(l_io::path("")));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST_("data",
  File *f1 = new File(l_io::path("")), *f2 = new File(l_io::path(""));
  CHECK_EQ(0, f1->idx->val);
  CHECK_EQ(0, f2->idx->val);

  Files fs;
  fs.addFile(l::sh(f1));
  fs.addFile(l::sh(f2));
  CHECK_EQ(1, f1->idx->val);
  CHECK_EQ(2, f2->idx->val);

  f1->addSet(l::sh(new Set(
    l::sh(new FileIdx),
    l::sh(new Meta(f1->dict, MetaVals(), 0, 0, 0, 0, 0, 0, 0, 0)),
    l::sh(new Image))));

  fs.remFile(0);
  CHECK_EQ(1, f2->idx->val);
)

//------------------------------------------------------------------------------
}}
// eof
