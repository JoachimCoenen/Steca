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
#include <c2/inc/c_cpp>

namespace core {
//------------------------------------------------------------------------------

count_arr2::count_arr2(c::sz2 sz) : cs(sz, 0) {}

count_arr2::count_arr2(rc that) : cs(that.size()) {
  for_i (cs.sz.size())
    setAt(i, that.at(i));
}


void count_arr2::addAt(uint i, uint j, inten_t c) {
  auto sum = (cs.refAt(i, j) += c);
  mut(rgeCount).extendBy(sum);
}

//------------------------------------------------------------------------------

Image::Image() : Image(c::sz2()) {}

Image::Image(c::sz2::rc sz) : intens(new count_arr2(sz)) {}

Image::Image(count_arr2::rc is): intens(new count_arr2(is)) {}

c::sz2 Image::size() const {
  return intens->size();
}

void Image::clear() {
  intens.reset(new count_arr2(c::sz2()));
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

void Image::setInten(uint i, uint j, inten_t val) {
  mut(*intens).setAt(i, j, val);
}

void Image::addInten(uint i, uint j, inten_t val) {
  auto &is = mut(*intens);
  is.addAt(i, j, val);
}

void Image::addIntens(rc that) {
  if (size() != that.size())
    c::err("inconsistent image size");

  auto sz = size();
  for_ij (c::to_uint(sz.i), c::to_uint(sz.j))
    addInten(i, j, that.inten(i, j));
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
