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

#include "sets.hpp"
#include "../calc/fit_params.hpp"
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>

namespace core { namespace data {
//------------------------------------------------------------------------------

MetaDictBase::MetaDictBase() : keys() {}
MetaDictBase::MetaDictBase(rc that) : keys(that.keys) {}

MetaDictBase::~MetaDictBase() {}

void MetaDictBase::clear() {
  mut(keys).clear();
  keySet.clear();
}

void MetaDictBase::enter(strc key) {
  if (keySet.contains(key))
    return;
  mut(keys).add(key);
  keySet.add(key);
}

void MetaDictBase::enter(str_vec::rc keys) {
  for (auto&& key : keys)
    enter(key);
}

//------------------------------------------------------------------------------

FilesMetaDict::FilesMetaDict() : base() {}
FilesMetaDict::FilesMetaDict(rc that) : base(that) {}

//------------------------------------------------------------------------------

MetaDict::MetaDict() : base(), idxs() {}
MetaDict::MetaDict(rc that) : base(that), idxs(that.idxs) {}

int MetaDict::safeIndex(strc key) const {
  return idxs.contains(key) ? int(idxs.at(key)) : -1;
}

void MetaDict::clear() {
  base::clear();
  mut(idxs).clear();
}

void MetaDict::enter(strc key) {
  idxEnter(key);
}

void MetaDict::enter(str_vec::rc keys) {
  base::enter(keys);
}

MetaDict::idx MetaDict::idxEnter(strc key) {
  auto it = idxs.find(key);
  if (idxs.end() != it)
    return it->second;

  auto i = idxs.size();
  mut(idxs).add(key, i);
  base::enter(key);
  ENSURE_(idxs.size() == keys.size())

  return i;
}

//------------------------------------------------------------------------------

flt32 MetaVals::valAt(idx i) const may_err {
  return at(i);
}

MetaVals::ref MetaVals::setAt(idx i, flt32 val) {
  if (contains(i))
    at(i) = val;
  else
    add(i, val);
  RTHIS
}

MetaVals::ref MetaVals::addAt(idx i, flt32 val) {
  if (contains(i))
    val += at(i);
  setAt(i, val);
  RTHIS
}

//------------------------------------------------------------------------------

Meta::Meta(MetaDict::shp dict_)
: comment()
, tth(0.), omg(0.), chi(0.), phi(0.), tim(0), mon(0) , dTim(0), dMon(0)
, dict(dict_), vals() {}

Meta::Meta(MetaDict::shp dict_, MetaVals::rc vals_,
           flt32 tth_, flt32 omg_, flt32 chi_,  flt32 phi_,
           flt32 tim_, flt32 mon_, flt32 dTim_, flt32 dMon_)
: comment()
, tth(tth_), omg(omg_), chi(chi_), phi(phi_)
, tim(tim_), mon(mon_) , dTim(dTim_), dMon(dMon_)
, dict(dict_), vals(vals_) {}

TEST_("dict",
  MetaDict dict;
  CHECK_EQ(0, dict.idxEnter("0"));
  CHECK_EQ(1, dict.idxEnter("1"));
  CHECK_EQ(0, dict.idxEnter("0"));
  CHECK_EQ(2, dict.idxEnter("2"));
)

#ifndef _WIN32 // CDB has some trouble with this
TEST_("dict-throw",
  MetaDict dict;
  CHECK_THROWS_AS(dict.index(""), std::exception);
)
#endif

//------------------------------------------------------------------------------

FileSrc::FileSrc(l_io::path::rc path_, strc comment_)
: path(path_), comment(comment_) {}

//------------------------------------------------------------------------------

using FitParams = calc::FitParams;

Set::Set(FileSrc::shp src_, Meta::shp meta_, Image::shp image_)
: src(src_), meta(meta_), image(image_) {}

l::sz2 Set::imageSize() const {
  EXPECT_(image)
  return image->size();
}

gma_rge Set::rgeGma(FitParams::rc fp) const {
  return fp.angleMap(*this)->rgeGma;
}

gma_rge Set::rgeGmaFull(FitParams::rc fp) const {
  return fp.angleMap(*this)->rgeGmaFull;
}

tth_rge Set::rgeTth(FitParams::rc fp)const {
  return fp.angleMap(*this)->rgeTth;
}

void Set::collect(FitParams::rc fp,
                core::inten_vec& intens, uint_vec& counts,
                gma_rge::rc rgeGma, tth_t minTth, tth_t deltaTth) const {

  auto&& map = *fp.angleMap(*this);

  // get min-max indexes of gammas that are in the gamma range
  uint_vec const* gmaIndexes = nullptr;
  uint gmaIndexMin = 0, gmaIndexMax = 0;
  map.getGmaIndexes(rgeGma, gmaIndexes, gmaIndexMin, gmaIndexMax);

  EXPECT_(gmaIndexes)
  EXPECT_(gmaIndexMin <= gmaIndexMax)
  EXPECT_(gmaIndexMax <= gmaIndexes->size())

  auto size = intens.size();
  EXPECT_(size == counts.size())

  EXPECT_(0 < deltaTth)

  auto* corrImg = fp.corrEnabled ? fp.corrImage.ptr() : nullptr;
  // for all applicable gammas
  for (auto i = gmaIndexMin; i < gmaIndexMax; ++i) {
    auto ind   = gmaIndexes->at(i);
    // get intensity
    auto inten = image->inten(ind);
    if (l::isnan(inten))
      continue;

    // get correction, or assume 1 (no correction)
    inten_t ci = corrImg ? corrImg->inten(ind) : inten_t(1);
    if (l::isnan(ci))
      continue;

    // correct
    inten *= ci;

    // figure out the bin index
    tth_t tth  = map.at(ind).tth;
    auto ti = l::to_uint(l::floor((tth - minTth) / deltaTth));
    EXPECT_(ti <= size)
    // the last one could overshoot due to floating point calculation
    ti = l::min(ti, size-1);

    // add it
    intens.refAt(ti) += inten;
    counts.refAt(ti) += 1;
  }
}

//------------------------------------------------------------------------------

Sets::Sets() : lazyFoldImage() {}

l::sz2 Sets::imageSize() const {
  EXPECT_(!isEmpty())
  // all images have the same size; simply take the first one
  return first()().imageSize();
}

Image::shp Sets::foldImage() const {
  if (lazyFoldImage)
    return lazyFoldImage;

  uint n = size();
  EXPECT_(0 < n)

  lazyFoldImage.reset(new Image(imageSize()));

  Image::ref d = mut(*lazyFoldImage);
  for_i_(n)
    d.addIntens(*(at(i)().image));

  return lazyFoldImage;
}

Sets::ref Sets::add(Set::shr set) may_err {
  check_or_err_(isEmpty() || first()().imageSize() == set().imageSize(),
                "Inconsistent image size");
  base::add(set);
  RTHIS;
}

//------------------------------------------------------------------------------

CombinedSet::CombinedSet(uint fileNo_)
: fileNo(fileNo_), tag(), isActive(true)
, lazyMeta()
, lazyOmg(l::flt64_nan), lazyPhi(l::flt64_nan), lazyChi(l::flt64_nan)
, lazyTim(l::flt32_nan), lazyMon(l::flt32_nan), lazyDTim(l::flt32_nan), lazyDMon(l::flt32_nan) {}

Meta::shp CombinedSet::meta() const {
  if (lazyMeta)
    return lazyMeta;

  uint n = size();
  EXPECT_(0 < n)

  if (1==n)
    return (lazyMeta = first()().meta);

  lazyMeta.reset(new Meta(l::sh(new MetaDict)));

  Meta::ref lm = mut(*lazyMeta);
  MetaDict::ref ld = mut(*lm.dict);
  MetaVals::rc lv = lm.vals;

  // max of: tim, mon
  // sum of: dTim, dMon
  // avg of: all else

  for_i_(n) {
    Meta::rc im = *(at(i)().meta);

    mut(lm.tth) = lm.tth + im.tth;
    mut(lm.omg) = lm.omg + im.omg;
    mut(lm.chi) = lm.chi + im.chi;
    mut(lm.phi) = lm.phi + im.phi;

    mut(lm.tim) = l::max(lm.tim, im.tim);
    mut(lm.mon) = l::max(lm.mon, im.mon);

    mut(lm.dTim) += l::isnan(im.dTim) ? 0.f : im.dTim;
    mut(lm.dMon) += l::isnan(im.dMon) ? 0.f : im.dMon;

    MetaDict::rc id = *im.dict;
    MetaVals::rc iv = im.vals;

    for (auto&& k : id.keys) {
      auto&& idx = id.index(k);
      auto&& val = iv.valAt(idx);
      auto&& idl = ld.idxEnter(k);
      mut(lv).addAt(idl, val);
    }
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
  if (l::isnan(lazyVal)) {          \
    EXPECT_(!isEmpty())             \
    for (auto&& set: *this)         \
      mut(lazyVal) += set().fun();  \
    mut(lazyVal) /= size();         \
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
  if (l::isnan(lazyVal)) {          \
    EXPECT_(!isEmpty())             \
    for (auto&& set: *this)         \
      mut(lazyVal) += set().fun();  \
  }

flt32 CombinedSet::dTim() const {
  SUM_SETS_VAL(lazyDTim, dTim)
  return lazyDTim;
}

flt32 CombinedSet::dMon() const {
  SUM_SETS_VAL(lazyDMon, dMon)
  return lazyDMon;
}

#define RGE_SETS_COMBINE(typ, op, fun)  \
  EXPECT_(!isEmpty())                   \
  typ rge;                              \
  for (auto&& set : *this)              \
    rge.op(set().fun);                  \
  return rge;

gma_rge CombinedSet::rgeGma(calc::FitParams::rc s) const {
  RGE_SETS_COMBINE(gma_rge, extendBy, rgeGma(s))
}

gma_rge CombinedSet::rgeGmaFull(FitParams const& s) const {
  RGE_SETS_COMBINE(gma_rge, extendBy, rgeGmaFull(s))
}

tth_rge CombinedSet::rgeTth(FitParams const& s) const {
  RGE_SETS_COMBINE(tth_rge, extendBy, rgeTth(s))
}

inten_rge CombinedSet::rgeInten() const {
  RGE_SETS_COMBINE(inten_rge, intersect, rgeInten())
}

inten_vec CombinedSet::collectIntens(FitParams::rc fp, gma_rge::rc rgeGma) const {
  auto tthRge = rgeTth(fp);
  auto tthWdt = tth_t(tthRge.width());

  auto cut = fp.geometry.imageCut;
  uint pixWidth = fp.geometry.imageSize.i - cut.left - cut.right;

  uint numBins;
  if (1 < size()) {  // combined datasets
    auto one   = first();
    auto delta = tth_t(one().rgeTth(fp).width() / pixWidth);
    numBins    = l::to_uint(l::ceil(tthWdt / delta));
  } else {
    numBins    = pixWidth; // simply match the number of pixels
  }

  inten_vec intens(numBins, inten_t(0));
  uint_vec  counts(numBins, 0);

  auto minTth   = tth_t(tthRge.min);
  auto deltaTth = tthWdt / real(numBins);

  for (auto&& one : *this)
    one().collect(fp, intens, counts, rgeGma, minTth, deltaTth);

  // sum or average
  if (fp.avgIntens) {
    auto scale = fp.intenScale;
    for_i_(numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens.refAt(i) *= (scale / cnt);
    }
  }

  return intens;
}

inten_vec CombinedSet::histogram(FitParams::rc fp, gma_rge::rc rgeGma) const {
  tth_rge tthRge = rgeTth(fp);            // the range of 2theta
  tth_t   tthWdt = tth_t(tthRge.width()); // its width

  // pixel width of the image
  auto cut = fp.geometry.imageCut;
  uint pixWidth = fp.geometry.imageSize.i - cut.left - cut.right;

  // the number of bins in the histogram:
  uint numBins;
  if (1 < size()) {
    // if more than one set (they can have different 2theta ranges),
    // calculate 2theta delta (from the first image, that's fine)
    auto one   = first();
    auto delta = tth_t(one().rgeTth(fp).width() / pixWidth);
    // the full 2theta width / 2theta delta
    numBins = l::to_uint(l::ceil(tthWdt / delta));
  } else {
    // if only one set, number of bins = pixel width of the image
    // (simply match the pixel resolution)
    numBins = pixWidth;
  }

  // intensities and counts
  inten_vec intens(numBins, inten_t(0));
  uint_vec  counts(numBins, 0);

  auto minTth = tth_t(tthRge.min), deltaTth = tthWdt / real(numBins);

  // collect (sum of) intensities (pixels) from images
  for (auto&& one : *this)
    one().collect(fp, intens, counts, rgeGma, minTth, deltaTth);

  // optionally, make averages
  if (fp.avgIntens) {
    auto scale = fp.intenScale;
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
  return first()().imageSize();
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

inten_rge::rc CombinedSets::rgeFixedInten(calc::FitParams const& fp, bool trans, bool cut) const {
  if (!lazyRgeFixedInten.isDef()) {
    l_io::busy __;

    for (auto&& set: *this)
      for (auto&& one : *set) {
        if (one().image) {
          auto&& image = *one().image;
          auto&& lens = fp.imageLens(image,*this,trans,cut);
          lazyRgeFixedInten.extendBy(lens().rgeInten(false));
        }
      }
  }

  return lazyRgeFixedInten;
}

CombinedSet::shr CombinedSets::combineAll() const {
  auto&& set = l::sh(new CombinedSet(0));
  for (auto&& cs : *this)
    for (auto&& s : cs())
      mut(set()).add(s);
  return set;
}

void CombinedSets::resetLazies() {
  lazyMon = lazyDTim = lazyDMon = l::flt32_nan;
  lazyRgeFixedInten = inten_rge();
}

//------------------------------------------------------------------------------

TEST_("data::shp",
//  File::shp f1(new File(l_io::path("a"))), f2(new File(l_io::path("b")));
//  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST_("data",
//  File *f1 = new File(l_io::path("a")), *f2 = new File(l_io::path("b"));

//  Files fs;
//  fs.addFile(l::sh(f1));
//  fs.addFile(l::sh(f2));

//  f1->addSet(l::sh(new Set(
//    f1->src,
//    l::sh(new Meta(f1->dict, MetaVals(), 0, 0, 0, 0, 0, 0, 0, 0)),
//    l::sh(new Image))));

//  fs.remFileAt(0);
)

//------------------------------------------------------------------------------
}}
// eof