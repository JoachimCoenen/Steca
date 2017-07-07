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

#include "session.hpp"
#include <dev_lib/defs.inc>
#include <dev_lib/io/log.hpp>
#include "fit/fit_methods.hpp"

namespace core {
//------------------------------------------------------------------------------

Session::Session()
: normStrLst({"none", "monitor", "Δ monitor", "Δ time", "background"})
, angleMapKey0()
, imageTransform(), imageCut(), imageSize()
, avgScaleIntens(), intenScale(1)
, corrEnabled(false), corrFile(), corrImage()
, bgPolyDegree(0), bgRanges()
, angleMapCache(l::pint(12)), intensCorrImage(), corrHasNaNs()
{}

AngleMap::sh Session::angleMap(data::Set::rc set) const {
  AngleMap::Key key(angleMapKey0, set.tth());
  auto map = angleMapCache.get(key);
  if (!map)
    map = angleMapCache.add(key, AngleMap::sh(new AngleMap(key)));
  return map;
}

Image::sh Session::intensCorr() const {
  if (!corrEnabled)
    return Image::sh();

  EXPECT_(intensCorrImage)
  if (intensCorrImage->isEmpty())
    calcIntensCorr();

  return intensCorrImage;
}

calc::ImageLens::sh Session::imageLens(
  Image::rc image, data::CombinedSets::rc datasets, bool trans, bool cut) const
{
  return l::share(new calc::ImageLens(*this, image, datasets, trans, cut));
}

Curve Session::makeCurve(calc::DatasetLens::rc lens, gma_rge::rc rgeGma) const {
  Curve curve = lens.makeCurve(rgeGma);
  curve.subtract(fit::Polynom::fromFit(bgPolyDegree, curve, bgRanges));

  return curve;
}

calc::DatasetLens::sh Session::datasetLens(
    data::CombinedSet::rc dataset, data::CombinedSets::rc datasets,
    eNorm norm, bool trans, bool cut) const {
  return l::share(new calc::DatasetLens(*this, dataset, datasets, norm,
                         trans, cut, imageTransform, imageCut));
}

real Session::calcAvgBackground(data::CombinedSet::rc dataset) const {
  EXPECT_(dataset.parent)
  auto lens = datasetLens(dataset, *dataset.parent, eNorm::NONE, true, true);

  Curve gmaCurve = lens->makeCurve(true); // REVIEW averaged?
  auto bgPolynom = fit::Polynom::fromFit(bgPolyDegree, gmaCurve, bgRanges);
  return bgPolynom.avgY(lens->rgeTth());
}

real Session::calcAvgBackground(data::CombinedSets::rc datasets) const {
  if (datasets.sets.isEmpty())
    return 0;

  l_io::busy __;

  real bg = 0;

  for (auto& dataset : datasets.sets)
    bg += calcAvgBackground(*dataset);

  return bg / datasets.sets.size();
}

void Session::calcIntensCorr() const {
  corrHasNaNs = false;

  EXPECT_(corrImage)
  l::sz2 size = corrImage->size() - imageCut.marginSize();
  ENSURE_(!size.isEmpty())

  uint w = size.i, h = size.j,
       di = imageCut.left, dj = imageCut.top;

  real sum = 0;
  for_ij_(w, h)
    sum += corrImage->inten(i + di, j + dj);

  real avg = sum / (w * h);

  intensCorrImage = l::share(new Image(corrImage->size(), 1));

  for_ij_(w, h) {
    auto  inten = corrImage->inten(i + di, j + dj);
    real factor;

    if (inten > 0) {
      factor = avg / inten;
    } else {
      factor = NAN;
      corrHasNaNs = true;
    }

    mutp(intensCorrImage)->setInten(i + di, j + dj, inten_t(factor));
  }
}

//------------------------------------------------------------------------------
}
// eof
