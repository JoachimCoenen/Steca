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

#include "lens.hpp"
#include <dev_lib/defs.inc>
#include <dev_lib/io/log.hpp>

#include "../session.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

LensBase::LensBase(Session::rc session_, data::CombinedSets::rc datasets_,
                   ImageTransform::rc imageTransform_, ImageCut::rc imageCut_,
                   bool trans_, bool cut_)
: session(session_), datasets(datasets_)
, imageTransform(imageTransform_), imageCut(imageCut_)
, trans(trans_), cut(cut_)
, intensCorr(session.intensCorr()) {
}

LensBase::~LensBase() {}

l::sz2 LensBase::transCutSize(l::sz2 size) const {
  if (trans && imageTransform.isTransposed())
    size = size.transposed();
  if (cut)
    size = size - imageCut.marginSize();

  return size;
}

void LensBase::doTrans(uint& i, uint& j) const {
  auto s = size();
  uint w = s.i;
  uint h = s.j;

  switch (imageTransform.val) {
  case ImageTransform::ROTATE_0:
    break;
  case ImageTransform::ROTATE_1:
    std::swap(i, j);
    j = w - j - 1;
    break;
  case ImageTransform::ROTATE_2:
    i = w - i - 1;
    j = h - j - 1;
    break;
  case ImageTransform::ROTATE_3:
    std::swap(i, j);
    i = h - i - 1;
    break;
  case ImageTransform::MIRROR_ROTATE_0:
    i = w - i - 1;
    break;
  case ImageTransform::MIRROR_ROTATE_1:
    j = h - j - 1;
    std::swap(i, j);
    j = w - j - 1;
    break;
  case ImageTransform::MIRROR_ROTATE_2:
    j = h - j - 1;
    break;
  case ImageTransform::MIRROR_ROTATE_3:
    std::swap(i, j);
    break;
  }
}

void LensBase::doCut(uint& i, uint& j) const {
  i += imageCut.left; j += imageCut.top;
}

//------------------------------------------------------------------------------

ImageLens::ImageLens(Session::rc session,
                     Image::rc image_, data::CombinedSets::rc datasets,
                     bool trans, bool cut)
: base(session, datasets, session.imageTransform, session.imageCut, trans, cut)
, image(image_)
{}

inten_t ImageLens::imageInten(uint i, uint j) const {
  if (trans) doTrans(i, j);
  if (cut)   doCut(i, j);

  auto inten = image.inten(i, j);
  if (intensCorr)
    inten *= intensCorr->inten(i, j);

  return inten;
}

inten_rge::rc ImageLens::rgeInten(bool fixed) const {
  if (fixed)
    return datasets.rgeFixedInten(session, trans, cut);

  if (!lazyRgeInten.isDef()) {
    auto sz = size();
    for_ij_(sz.i, sz.j)
      lazyRgeInten.extendBy(imageInten(i, j));
  }

  return lazyRgeInten;
}

//------------------------------------------------------------------------------

DatasetLens::DatasetLens(Session::rc session,
   data::CombinedSet::rc dataset_, data::CombinedSets::rc datasets, eNorm norm,
   bool trans, bool cut, ImageTransform::rc imageTransform, ImageCut::rc imageCut)
: base(session, datasets, imageTransform, imageCut, trans, cut)
, normFactor(1), dataset(dataset_) {
  setNorm(norm);
}

l::sz2 DatasetLens::size() const {
  return base::transCutSize(datasets.imageSize());
}

gma_rge DatasetLens::rgeGma() const {
  return dataset.rgeGma(session);
}

gma_rge DatasetLens::rgeGmaFull() const {
  return dataset.rgeGmaFull(session);
}

tth_rge DatasetLens::rgeTth() const {
  return dataset.rgeTth(session);
}

inten_rge DatasetLens::rgeInten() const {
  // "freezes" the scale
  // TODO consider return datasets_.rgeInten();
  return dataset.rgeInten();
}

Curve DatasetLens::makeCurve() const {
  return makeCurve(rgeGma());
}

Curve DatasetLens::makeCurve(gma_rge::rc rgeGma) const {
  inten_vec intens = dataset.collectIntens(session, intensCorr, rgeGma);

  Curve res;
  uint size = intens.size();

  if (size) {
    auto rgeTth   = dataset.rgeTth(session);
    auto minTth   = tth_t(rgeTth.min);
    auto deltaTth = tth_t(rgeTth.width() / size);
    for_i_(size)
      res.add(minTth + deltaTth * real(i), intens.at(i) * normFactor);
  }

  return res;
}

void DatasetLens::setNorm(eNorm norm) {
  real num = 1, den = 1;

  switch (norm) {
  case eNorm::MONITOR:
    num = datasets.mon();
    den = dataset.mon();
    break;
  case eNorm::DELTA_MONITOR:
    num = datasets.dMon();
    den = dataset.dMon();
    break;
  case eNorm::DELTA_TIME:
    num = datasets.dTim();
    den = dataset.dTim();
    break;
  case eNorm::BACKGROUND:
    num = session.calcAvgBackground(datasets);
    den = session.calcAvgBackground(dataset);
    break;
  case eNorm::NONE:
    break;
  }

  normFactor = inten_t((num > 0 && den > 0) ? num / den : l::flt_nan);
  if (l::isnan(normFactor))
    l_io::log::warn("Bad normalisation value");
}

//------------------------------------------------------------------------------
}}
// eof
