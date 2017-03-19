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
#include <c2/h/c_cpp>

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

Meta::Dicts::Dicts() : dictFlt(), dictStr() {}

Meta::Meta(Dicts::sh dicts_, flt32 tth_, flt32 omg_, flt32 chi_, flt32 phi_)
: dicts(dicts_), valsFlt(), valsStr()
, tth(tth_), omg(omg_), chi(chi_), phi(phi_)
{
  mut(valsFlt).reserve(dicts->dictFlt.size());
  mut(valsStr).reserve(dicts->dictStr.size());
}

TEST("dict",
  Meta::Dict dict;
  CHECK_EQ(0, dict.size());

  CHECK_EQ(0, dict.add("0"));
  CHECK_EQ(1, dict.add("1"));
  CHECK_EQ(0, dict.add("0"));
  CHECK_EQ(2, dict.add("2"));

  CHECK_EQ(3, dict.size());

  CHECK_THROWS_AS(dict.at(""), c::exc::rc);
)

//------------------------------------------------------------------------------

Set::Set(Meta::sh meta_, Image::sh image_)
: idx(0), meta(meta_), image(image_) {}

tth_t Set::midTth() const {
  return real(meta->tth);
}

c::deg Set::omg() const {
  return real(meta->omg);
}

c::deg Set::phi() const {
  return real(meta->tth);
}

c::deg Set::chi() const {
  return real(meta->tth);
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

void Set::collect(Session::rc s, Image const* corrImage,
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

    inten_t corr = corrImage ? corrImage->inten(ind) : 1;
    if (c::isnan(corr))
      continue;

    inten *= corr;

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

File::File() : idx(0), sets() {}

void File::addSet(Set::sh set) {
  mut(sets).add(set);
}

//------------------------------------------------------------------------------

Files::Files() : files(), dicts(new Meta::Dicts) {}

void Files::addFile(c::give_me<File> file) {
  mut(files).add(File::sh(file));
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
  File::sh f1(new File()), f2(new File);
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST("data",
  Files fs;

  File *f1 = new File, *f2 = new File;
  CHECK_EQ(0, f1->idx);
  CHECK_EQ(0, f2->idx);

  fs.addFile(c::give_me<File>::from(f1));
  fs.addFile(c::give_me<File>::from(f2));
  CHECK_EQ(1, f1->idx);
  CHECK_EQ(2, f2->idx);

  f1->addSet(new Set(c::share(new Meta(fs.dicts, 0, 0, 0, 0)),
                     c::share(new Image)));
  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof
