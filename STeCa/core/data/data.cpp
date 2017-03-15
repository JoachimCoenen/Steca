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
#include <c2/c/c_cpp>
#include <c2/c/lib/num.h>

namespace core { namespace data {
//------------------------------------------------------------------------------

Meta::Meta(Dict::sh dict_, float tth_, float omg_, float chi_, float phi_)
: dict(dict_), vals(), tth(tth_), omg(omg_), chi(chi_), phi(phi_) {
  mut(vals).reserve(dict->size());
}

uint Meta::Dict::size() const {
  return base::size();
}

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

Set::Set(Meta::rc meta_) : idx(0), meta(meta_) {
}

//------------------------------------------------------------------------------

File::File() : idx(0), sets() {
}

File::~File() {
}

void File::addSet(Set::sh set) {
  mut(sets).push(set);
}

//------------------------------------------------------------------------------

Files::Files() : files(), dict(new Meta::Dict) {
}

void Files::addFile(c::give_me<File> file) {
  mut(files).push(File::sh(file));
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

  Meta meta(fs.dict, 0, 0, 0, 0);

  f1->addSet(new Set(meta));

  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof
