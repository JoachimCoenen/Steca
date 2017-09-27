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

#include "fit_params.hpp"
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>

namespace core { namespace calc {
//------------------------------------------------------------------------------

FitParams::FitParams()
: corrEnabled(false), bgPolyDegree(0), bgRanges(), bg(), refls(), intenScale(1), geometry(), avgScaleIntens(false), angleMapCache(l::pint(12)) {}

AngleMap::shp FitParams::angleMap(data::Set::rc set) const {
//  Key(Geometry::rc, l::sz2::rc, ImageCut::rc, l::ij::rc midPix, tth_t midTth);
  AngleMap::Key key(geometry, set.tth());
  auto map = angleMapCache.get(key);
  if (!map)
    map = angleMapCache.add(key, AngleMap::shp(new AngleMap(key)));
  return map;
}

ImageLens::shr FitParams::imageLens(
    Image::rc image, data::CombinedSets::rc datasets, bool trans, bool cut) const
{
  return l::sh(new ImageLens(*this, image, datasets, trans, cut));
}

Curve FitParams::makeCurve(calc::DatasetLens::rc lens, gma_rge::rc rgeGma) const {
  Curve curve = lens.makeCurve(rgeGma);
  curve.subtract(fit::Polynom::fromFit(bgPolyDegree, curve, bgRanges));

  return curve;
}

Image::shp FitParams::intensCorr() const {
  if (!corrEnabled)
    return Image::shp();

  if (!intensCorrImage)
    calcIntensCorr();

  return intensCorrImage;
}

DatasetLens::shr FitParams::datasetLens(
    data::CombinedSets::rc datasets, data::CombinedSet::rc dataset,
    bool trans, bool cut) const {
  return l::sh(new calc::DatasetLens(*this, datasets, dataset, norm,
                         trans, cut));
}

real FitParams::calcAvgBackground(data::CombinedSets::rc datasets, data::CombinedSet::rc dataset) const {
  auto lens = datasetLens(datasets, dataset, true, true);

  Curve gmaCurve = lens().makeCurve(true); // REVIEW averaged?
  auto bgPolynom = fit::Polynom::fromFit(bgPolyDegree, gmaCurve, bgRanges);
  return bgPolynom.avgY(lens().rgeTth());
}

real FitParams::calcAvgBackground(data::CombinedSets::rc datasets) const {
  if (datasets.isEmpty())
    return 0;

  l_io::busy __;

  real bg = 0;

  for (auto&& dataset : datasets)
    bg += calcAvgBackground(datasets, *dataset);

  return bg / datasets.size();
}

void FitParams::calcIntensCorr() const {
  corrHasNaNs = false;

  EXPECT_(corrImage)
  l::sz2 size = corrImage->size() - geometry.imageCut.marginSize();
  ENSURE_(!size.isEmpty())

  uint w = size.i, h = size.j,
       di = geometry.imageCut.left, dj = geometry.imageCut.top;

  real sum = 0;
  for_ij_(w, h)
    sum += corrImage->inten(i + di, j + dj);

  real avg = sum / (w * h);

  intensCorrImage = l::sh(new Image(corrImage->size(), inten_t(1)));

  for_ij_(w, h) {
    auto inten = corrImage->inten(i + di, j + dj);
    real factor;

    if (inten > 0) {
      factor = avg / inten;
    } else {
      factor = l::flt_nan;
      corrHasNaNs = true;
    }

    mut(*intensCorrImage).setInten(i + di, j + dj, inten_t(factor));
  }
}

//------------------------------------------------------------------------------
}}
// eof
