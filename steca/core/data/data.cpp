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

#include "data.h"
#include "../session.h"
#include <app_lib/inc/defs_cpp.h>
#include <app_lib/inc/exc.h>

namespace core { namespace data {
//------------------------------------------------------------------------------

uint Meta::Dict::add(strc key) {
  auto it = base::find(key);
  if (base::end() == it)
    it = base::insert(key, size());
  return it.value();
}

uint Meta::Dict::at(strc key) const may_err {
  auto it = base::find(key);
  if (base::end() == it)
    err(str("Dict has no %1").arg(key));
  return it.value();
}

Meta::Meta(Dict::shc dict_)
: comment()
, dict(dict_), vals(dict->size(), 0), tth(0), omg(0), chi(0), phi(0)
, tim(0), mon(0) , dTim(0), dMon(0) {}

Meta::Meta(Dict::shc dict_, flt_vec const& vals_,
           flt32 tth_, flt32 omg_, flt32 chi_,  flt32 phi_,
           flt32 tim_, flt32 mon_, flt32 dTim_, flt32 dMon_)
: comment()
, dict(dict_), vals(vals_), tth(tth_), omg(omg_), chi(chi_), phi(phi_)
, tim(tim_), mon(mon_) , dTim(dTim_), dMon(dMon_) {
  EXPECT_(dict->size() == vals.size())
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

#ifndef Q_OS_WIN // CDB has some trouble with this
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

  EXPECT_(gmaIndexes)
  EXPECT_(gmaIndexMin <= gmaIndexMax)
  EXPECT_(gmaIndexMax <= l::to_u(gmaIndexes->size()))

  auto size = intens.size();
  EXPECT_(size == counts.size())

  EXPECT_(0 < deltaTth)

  for (auto i = gmaIndexMin; i < gmaIndexMax; ++i) {
    auto ind   = gmaIndexes->at(i);
    auto inten = image->inten(ind);
    if (qIsNaN(inten))
      continue;

    inten_t ci = corr ? corr->inten(ind) : 1;
    if (qIsNaN(ci))
      continue;

    inten *= ci;

    tth_t tth  = map.at(ind).tth;

    // bin index
    auto ti = qFloor((tth - minTth) / deltaTth);
    EXPECT_(ti <= size)
    ti = qMin(ti, size-1); // it can overshoot due to floating point calculation

    intens[ti] += inten;
    counts[ti] += 1;
  }
}

//------------------------------------------------------------------------------

CombinedSet::CombinedSet() : sets()
, lazyOmg(qQNaN()), lazyPhi(qQNaN()), lazyChi(qQNaN())
, lazyTim(qQNaN()), lazyMon(qQNaN()), lazyDTim(qQNaN()), lazyDMon(qQNaN()) {}

Meta::sh CombinedSet::meta() const {
  if (lazyMeta)
    return lazyMeta;

  // max tim, mon
  // sum dTim, dMon
  // avg all else

  uint n = sets.size();
  EXPECT_(0 < n)

  mut(lazyMeta).reset(new Meta(sets.first()->meta->dict));

  Meta::ref d = mut(*lazyMeta);
  for_i_(n) {
    Meta::rc s = *(sets.at(i)->meta);

    EXPECT_(d.vals.size() == s.vals.size())
    for_i_(d.vals.size())
      mut(d.vals)[i] = d.vals.at(i) + s.vals.at(i);

    mut(d.tth) = d.tth + s.tth;
    mut(d.omg) = d.omg + s.omg;
    mut(d.chi) = d.chi + s.chi;
    mut(d.phi) = d.phi + s.phi;

    mut(d.tim) = qMax(d.tim, s.tim);
    mut(d.mon) = qMax(d.mon, s.mon);

    mut(d.dTim) += qIsNaN(s.dTim) ? 0.f : s.dTim;
    mut(d.dMon) += qIsNaN(s.dMon) ? 0.f : s.dMon;
  }

  real fac = 1.0 / n;

  for_i_(d.vals.size())
    mut(d.vals)[i] = inten_t(d.vals.at(i) * fac);

  mut(d.tth) = d.tth * fac;
  mut(d.omg) = d.omg * fac;
  mut(d.chi) = d.chi * fac;
  mut(d.phi) = d.phi * fac;

  return lazyMeta;
}

Image::sh CombinedSet::image() const {
  if (lazyImage)
    return lazyImage;

  uint n = sets.size();
  EXPECT_(0 < n)

  mut(lazyImage).reset(new Image(sets.first()->image->size()));

  Image::ref d = mut(*lazyImage);
  for_i_(n)
    d.addIntens(*(sets.at(i)->image));

  return lazyImage;
}

#define AVG_SETS_VAL(lazyVal, fun)  \
  if (qIsNaN(lazyVal)) {            \
    EXPECT_(!sets.isEmpty())        \
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
  if (qIsNaN(lazyVal)) {            \
    EXPECT_(!sets.isEmpty())        \
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
  EXPECT_(!sets.isEmpty())          \
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
    numBins = l::to_uint(qCeil(tthWdt / delta));
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
    for_i_(numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens[i] *= flt32(scale/cnt);
    }
  }

  return intens;
}

//------------------------------------------------------------------------------

File::File(Files::rc files_, strc name_)
: files(files_), idx(0), name(name_), comment(), strs(), sets() {}

File::ref File::addSet(Set::sh set) {
  mut(sets).append(set);
  return *this;
}

//------------------------------------------------------------------------------

Files::Files() : files(), dict(new Meta::Dict) {}

Files::ref Files::addFile(data::File::sh file) {
  EXPECT_(this == &file->files)
  EXPECT_(! // not there
    ([&]() {
      for_i_(files.size())
        if (file == files.at(i))
          return true;
      return false;
     }())
  )

  mut(files).append(file);
  mut(file->idx) = files.size();
  return *this;
}

Files::ref Files::remFile(uint i) {
  File::sh file = files.at(i);
  mut(files).remove(i);

  // renumber
  mut(file->idx) = 0;
  for_i_(files.size())
    mut(files.at(i)->idx) = i + 1;

  return *this;
}

//------------------------------------------------------------------------------

TEST("data::sh",
  Files fs;
  File::sh f1(new File(fs, "")), f2(new File(fs, ""));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST("data",
  Files fs;

  File *f1 = new File(fs, ""), *f2 = new File(fs, "");
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
