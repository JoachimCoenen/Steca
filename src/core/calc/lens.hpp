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

#pragma once
#include "../data/sets.hpp"
#include "../typ/curve.hpp"
#include "../typ/geometry.hpp"
#include "../typ/options.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------
// View the dataset through a lens (thanks, Antti!)

dcl_base_(LensBase)
  LensBase(FitParams const&, data::CombinedSets::rc, bool trans, bool cut);
  virtual mth_(l::sz2, size, ()) = 0;

protected:
  mth_(l::sz2, transCutSize, (l::sz2));

  voi_(doTrans, (uint& i, uint& j));
  voi_(doCut,   (uint& i, uint& j));

  atr_(bool, trans); // TODO remove ?
  atr_(bool, cut);

  ref_(FitParams,          fp);
  ref_(data::CombinedSets, datasets);
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(ImageLens, LensBase) SHARED
  ImageLens(FitParams const&, Image::rc, data::CombinedSets::rc, bool trans, bool cut);

  mth_(l::sz2, size, ()) RET_(base::transCutSize(image.size()))
  mth_(inten_t, imageInten, (uint i, uint j));

  inten_rge::rc rgeInten(bool fixed) const;

private:
  Image::rc image;
  mutable inten_rge lazyRgeInten;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(DatasetLens, LensBase) SHARED
  DatasetLens(FitParams const&, data::CombinedSet::rc, data::CombinedSets::rc,
              eNorm, bool trans, bool cut);

  mth_(l::sz2, size, ());

  mth_(gma_rge,   rgeGma,     ());
  mth_(gma_rge,   rgeGmaFull, ());
  mth_(tth_rge,   rgeTth,     ());
  mth_(inten_rge, rgeInten,   ());

  mth_(Curve, makeCurve, ());
  mth_(Curve, makeCurve, (gma_rge::rc));

//  data::Dataset::rc dataset()  const { return dataset_;   }

private:
  mut_(setNorm, (eNorm));
  inten_t normFactor;
  data::CombinedSet::rc dataset;
dcl_end

//------------------------------------------------------------------------------
}}
// eof
