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

#include "image.hpp"
#include <dev_lib/inc/defs.inc>

namespace core {
//------------------------------------------------------------------------------

count_arr2::count_arr2(l::sz2 sz) : cs(sz, 0) {}

count_arr2::count_arr2(rc that) : cs(that.size()) {
  for_i_(cs.sz.size())
    setAt(i, that.at(i));
}


count_arr2::ref count_arr2::addAt(uint i, uint j, inten_t c) {
  auto sum = (cs.refAt(i, j) += c);
  mut(rgeCount).extendBy(sum);
  return *this;
}

//------------------------------------------------------------------------------

Image::Image() : Image(l::sz2()) {}

Image::Image(l::sz2::rc sz) : intens(new count_arr2(sz)) {}

Image::Image(count_arr2::rc is): intens(new count_arr2(is)) {}

l::sz2 Image::size() const {
  return intens->size();
}

Image::ref Image::clear() {
  intens.reset(new count_arr2(l::sz2()));
  return *this;
}

bool Image::isEmpty() const {
  return intens->isEmpty();
}

inten_t Image::inten(uint i, uint j) const {
  return intens->at(i, j);
}

inten_t Image::inten(uint i) const {
  return intens->at(i);
}

Image::ref Image::setInten(uint i, uint j, inten_t val) {
  mut(*intens).setAt(i, j, val);
  return *this;
}

Image::ref Image::addInten(uint i, uint j, inten_t val) {
  auto &is = mut(*intens);
  is.addAt(i, j, val);
  return *this;
}

Image::ref Image::addIntens(rc that) {
  if (size() != that.size())
    l::err("inconsistent image size");

  auto sz = size();
  for_ij_(l::to_uint(sz.i), l::to_uint(sz.j))
    addInten(i, j, that.inten(i, j));

  return *this;
}

//------------------------------------------------------------------------------

ImageTransform::ImageTransform(uint val_) : val(eTransform(val_ & 7)) {}

ImageTransform ImageTransform::mirror(bool on) const {
  return on ? ImageTransform(val | MIRROR) : ImageTransform(val & ~MIRROR);
}

ImageTransform ImageTransform::rotateTo(ImageTransform::rc rot) const {
  return ImageTransform((val & MIRROR) | (rot.val & 3));
}

ImageTransform ImageTransform::nextRotate() const {
  return rotateTo(val + 1);
}

//------------------------------------------------------------------------------
}
// eof
