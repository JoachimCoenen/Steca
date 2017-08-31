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

#include "files.hpp"
#include <lib/dev/inc/defs.inc>

namespace core { namespace data {
//------------------------------------------------------------------------------

File::File(Files::rc files_, l_io::path::rc path_)
: files(files_), idx(new Idx()), isActive(true)
, path(path_), name(path.filename()), comment(), sets() {}

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
  mut(file->idx->val) = size();
  return *this;
}

Files::ref Files::remFile(uint i) {
  File::sh file = at(i);
  rem(i);

  // renumber
  mut(file->idx->val) = 0;
  for_i_(size())
    mut(at(i)->idx->val) = i + 1;

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
  CHECK_EQ(0, f1->idx->val);
  CHECK_EQ(0, f2->idx->val);

  fs.addFile(l::sh(f1));
  fs.addFile(l::sh(f2));
  CHECK_EQ(1, f1->idx->val);
  CHECK_EQ(2, f2->idx->val);

  f1->addSet(l::sh(new Set(
    l::sh(new Idx),
    l::sh(new Meta(fs.dict, Meta::Vals(), 0, 0, 0, 0, 0, 0, 0, 0)),
    l::sh(new Image))));

  fs.remFile(0);
  CHECK_EQ(1, f2->idx->val);
)

//------------------------------------------------------------------------------
}}
// eof
