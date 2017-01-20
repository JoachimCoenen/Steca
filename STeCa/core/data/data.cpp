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
#include <c/c/cpp>

namespace core { namespace data {
//------------------------------------------------------------------------------

Set::Set() : idx(0) {
}

//------------------------------------------------------------------------------

File::File() : idx(0), sets() {
  TR("+F" << this)
}

File::~File() {
  TR("-F" << this)
}

//------------------------------------------------------------------------------

Files::Files() : files() {
}

void Files::addFile(c::give_me<File> file) {
  mut(files).push(File::sh(file));
  mut(file->idx) = uint(files.size());
}

void Files::remFile(uint i) {
  File::sh file = files.at(i);
  mut(files).rem(i);

  // renumber
  mut(file->idx) = 0;
  for_i (uint(files.size()))
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

  fs.remFile(0);
  CHECK_EQ(1, f2->idx);
)

//------------------------------------------------------------------------------
}}
// eof
