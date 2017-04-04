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

Meta::Meta(Dict::sh dict_, flt_vec::rc vals_,
           flt32 tth_, flt32 omg_, flt32 chi_, flt32 phi_,
           flt32 tim_, flt32 mon_)
: dict(dict_), vals(vals_), tth(tth_), omg(omg_), chi(chi_), phi(phi_)
, tim(tim_), mon(mon_) {}

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

File::File(Files::rc files_) : files(files_), idx(0), strs(), sets() {}

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
  File::sh f1(new File(fs)), f2(new File(fs));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST("data",
  Files fs;

  File *f1 = new File(fs), *f2 = new File(fs);
  CHECK_EQ(0, f1->idx);
  CHECK_EQ(0, f2->idx);

  fs.addFile(c::share(f1));
  fs.addFile(c::share(f2));
  CHECK_EQ(1, f1->idx);
  CHECK_EQ(2, f2->idx);

  f1->addSet(c::share(new Set(c::share(new Meta(fs.dict, flt_vec(), 0, 0, 0, 0, 0, 0)),
                              c::share(new Image))));
  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof
