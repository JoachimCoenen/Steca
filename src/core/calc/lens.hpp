/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

/** @file
 * Data as seen through "lenses" that do transformations. (Thanks, Antti!)
 */

#pragma once

#include "../data/sets.hpp"
#include "../typ/curve.hpp"
#include "../typ/geometry.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

enum class eNorm {
  NONE,
  MONITOR, DELTA_MONITOR, DELTA_TIME, BACKGROUND,
};

//------------------------------------------------------------------------------

/** Lens base */
dcl_base_(LensBase)
  LensBase(FitParams const&, data::CombinedSets::rc, bool trans, bool cut);
  virtual mth_(l::sz2, size, ()) = 0;

protected:
  /// optionally transform and adjust size by cut
  mth_(l::sz2, transCutSize, (l::sz2));

  /// optionally swap @c i and @c j based on transform
  voi_(doTrans, (uint& i, uint& j));
  /// optionally adjust @c i and @c j based on cut
  voi_(doCut,   (uint& i, uint& j));

  /// fitting parameters - to access geometry
  ref_(FitParams,          fp);
  /// datasets contain values pertinent to the whol set
  ref_(data::CombinedSets, datasets);
  /// is transposed
  atr_(bool, trans);
  /// has a cut
  atr_(bool, cut);
dcl_end

//------------------------------------------------------------------------------

/** A lens to look through at images
 */
dcl_sub_(ImageLens, LensBase) SHARED
  ImageLens(FitParams const&, Image::rc, data::CombinedSets::rc, bool trans, bool cut);

  /// image size as seen through the lens
  mth_(l::sz2, size, ()) RET_(base::transCutSize(image.size()))
  /// get intensity at coordinates
  mth_(inten_t, imageInten, (uint i, uint j));
  /// intensity range
  inten_rge::rc rgeInten(bool fixed) const;

private:
  Image::rc image;
  mutable inten_rge lazyRgeInten;
dcl_end

//------------------------------------------------------------------------------

/** A lens to look through at dataset
 */
dcl_sub_(DatasetLens, LensBase) SHARED
  DatasetLens(FitParams const&, data::CombinedSets::rc, data::CombinedSet::rc,
              eNorm, bool trans, bool cut);

  /// image size as seen through the lens
  mth_(l::sz2, size, ());

  mth_(gma_rge,   rgeGma,     ());
  mth_(gma_rge,   rgeGmaFull, ());
  mth_(tth_rge,   rgeTth,     ());
  mth_(inten_rge, rgeInten,   ());

  mth_(Curve, makeCurve, ());
  mth_(Curve, makeCurve, (gma_rge::rc));

private:
  mut_(setNorm, (eNorm));
  inten_t normFactor;
  data::CombinedSet::rc dataset;
dcl_end

//------------------------------------------------------------------------------
}}
// eof
