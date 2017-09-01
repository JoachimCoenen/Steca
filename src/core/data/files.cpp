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

File::File(l_io::path::rc path_)
: idx(new FileIdx()), isActive(true)
, path(path_), name(path.filename()), comment(), sets(), dict(new MetaDict) {}

File::ref File::addSet(Set::sh set) may_err {
  mut(sets).add(set);
  return *this;
}

l::sz2 File::imageSize() const {
  EXPECT_(!sets.isEmpty())
  return sets.imageSize();
}

//------------------------------------------------------------------------------

Files::Files() : dict(new FilesMetaDict) {}

static l::set<str> metaKeys(Files::rc files) {
  l::set<str> keys;

  for (auto&& f : files)
    for (auto&& k : f->dict->keys)
      keys.add(k);

  return keys;
}

Files::ref Files::addFile(data::File::sh file) {
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

  mutp(dict)->update(metaKeys(*this));

  return *this;
}

Files::ref Files::remFile(uint i) {
  File::sh file = at(i);
  rem(i);

  // renumber
  mut(file->idx->val) = 0;
  for_i_(size())
    mut(at(i)->idx->val) = i + 1;

  mutp(dict)->update(metaKeys(*this));

  return *this;
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
