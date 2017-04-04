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

Image::Image() : Image(c::sz2()) {}

Image::Image(c::sz2::rc sz) : intens(new inten_arr(sz, 0)) {}

Image::Image(inten_arr::rc that): Image(that.sz) {
  addIntens(that);
}

c::sz2 Image::size() const {
  return intens->sz;
}

void Image::clear() {
  intens.reset(new inten_arr(c::sz2(), 0));
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
  is.refAt(i, j) += val;
}

void Image::addIntens(rc that) {
  if (size() != that.size())
    c::err("inconsistent image size");

  auto sz = size();
  for_ij (c::to_uint(sz.i), c::to_uint(sz.j)) {
    auto inten = that.inten(i, j);
    mut(rgeInten).extendBy(real(inten));
    addInten(i, j, inten);
  }
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
