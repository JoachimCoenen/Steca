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

File::File(l_io::path::rc path)
: src(new FileSrc(path, str::null)), isActive(true)
, sets(), dict(new MetaDict) {}

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
Files::Files(rc that) : base(that), dict(that.dict->clone()) {}

static l::set<str> metaKeys(Files::rc files) {
  l::set<str> keys;

  for (auto&& f : files)
    for (auto&& k : f->dict->keys)
      keys.add(k);

  return keys;
}

bool Files::hasPath(l_io::path::rc path) const {
  for (auto&& file : *this)
    if (file->src->path == path)
      return true;
  return false;
}


void Files::addFile(data::File::sh file) {
  EXPECT_(!hasPath(file->src->path))
  add(file);
  mut(*dict).enter(metaKeys(*this));
}

void Files::remFileAt(uint i) {
  File::sh file = at(i);
  rem(i);
  mut(*dict).enter(metaKeys(*this));
}

CombinedSets::sh Files::collectDatasets(l::pint groupedBy) const {
  auto css = l::sh(new data::CombinedSets);

  uint i = 0, fileNo = 1;
  auto cs = l::scope(new data::CombinedSet(fileNo));

  auto appendCs = [&]() {
    auto sz = cs->size();
    if (!sz)
      return;

    str tag = str::num(i + 1); i += sz;
    if (groupedBy > 1)
      tag += '-' + str::num(i);
    mut(cs->tag)    = tag;

    mut(*css).add(cs.takeOwn());
    cs.reset(new data::CombinedSet(fileNo));
  };

  auto gb = groupedBy;
  for (auto&& file : *this) {
    if (file->isActive)
      for (auto&& set : file->sets) {
        cs->add(set);
        if (0 == --gb) {
          appendCs();
          gb = groupedBy;
        }
      }
    ++fileNo;
  }

  appendCs();  // the potentially remaining ones
  return css;
}

//------------------------------------------------------------------------------

TEST_("data::sh",
  File::sh f1(new File(l_io::path("a"))), f2(new File(l_io::path("b")));
  f2 = f2; f1 = f2; f2 = f1; f1 = f1;
)

TEST_("data",
  File *f1 = new File(l_io::path("a")), *f2 = new File(l_io::path("b"));
  CHECK_EQ("a", f1->src->path);
  CHECK_EQ("b", f2->src->path);

  Files fs;
  fs.addFile(l::sh(f1));
  fs.addFile(l::sh(f2));

  f1->addSet(l::sh(new Set(
    f1->src,
    l::sh(new Meta(f1->dict, MetaVals(), 0, 0, 0, 0, 0, 0, 0, 0)),
    l::sh(new Image))));

  fs.remFileAt(0);
)

//------------------------------------------------------------------------------
}}
// eof
