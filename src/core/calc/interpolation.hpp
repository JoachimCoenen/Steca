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
 * Antti's reverse-engineered reflection interpolation.
 */
#pragma once

#include "../data/sets.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

dcl_(ReflectionInfo)
  atr_(data::Meta::shp, md);

  atr_(l::deg,  alpha);
  atr_(l::deg,  beta);
  atr_(gma_rge, rgeGma);

  atr_(inten_t, inten); atr_(inten_t, intenError);
  atr_(tth_t,   tth);   atr_(tth_t,   tthError);
  atr_(fwhm_t,  fwhm);  atr_(fwhm_t,  fwhmError);

  ReflectionInfo(l::deg alpha, l::deg beta);
  ReflectionInfo(l::deg alpha, l::deg beta, gma_rge,
                 inten_t, inten_t /*error*/, tth_t, tth_t /*error*/, fwhm_t, fwhm_t /*error*/);
  ReflectionInfo(data::Meta::shp,
                 l::deg alpha, l::deg beta, gma_rge,
                 inten_t, inten_t /*error*/, tth_t, tth_t /*error*/, fwhm_t, fwhm_t /*error*/);

//class ReflectionInfo final {
//  enum class eField {
//    ALPHA, BETA, GAMMA1, GAMMA2,
//    INTEN, INTEN_ERROR,
//    TTH,   INTEN_TTH,
//    FWHM,  INTEN_FWHM,
//  };

//  enum class eReflAttr {
//    ALPHA, BETA, GAMMA1, GAMMA2,
//    INTEN, SIGMA_INTEN, TTH, SIGMA_TTH, FWHM, SIGMA_FWHM,
//    NUM_REFL_ATTR,
//  };

//  static str_vec      dataTags(bool out);
//  static typ::cmp_vec dataCmps();

//  static str const reflStringTag(uint attr, bool out);

//  ReflectionInfo();

//  ReflectionInfo(data::shpp_Metadata, typ::deg alpha, typ::deg beta, gma_rge);

//  typ::deg alpha()      const { return alpha_;  }
//  typ::deg beta()       const { return beta_;   }

//  gma_rge  rgeGma()     const { return rgeGma_; }

//  inten_t  inten()      const { return inten_;      }
//  inten_t  intenError() const { return intenError_; }

//  tth_t    tth()        const { return tth_;        }
//  tth_t    tthError()   const { return tthError_;   }

//  fwhm_t   fwhm()       const { return fwhm_;       }
//  fwhm_t   fwhmError()  const { return fwhmError_;  }

//  typ::row_t data() const;

//_private

dcl_end

////------------------------------------------------------------------------------

dcl_sub_(ReflectionInfos, l::vec<ReflectionInfo>)
//  CLASS(ReflectionInfos) SUPER(typ::vec<ReflectionInfo>)
//public:
//  ReflectionInfos();

//  void append(ReflectionInfo::rc);

//  inten_t       averageInten() const;
//  inten_rge::rc rgeInten()     const;

//_private
//  void invalidate();

//  mutable inten_t   avgInten_;
//  mutable inten_rge rgeInten_;
dcl_end

//------------------------------------------------------------------------------

dcl_(itf_t)
  atr_(inten_t, inten);
  atr_(tth_t,   tth);
  atr_(fwhm_t,  fwhm);

  itf_t();
  itf_t(inten_t, tth_t, fwhm_t);

  set_(operator=,  (rc));
  set_(operator+=, (rc));
dcl_end

using itfs_t = l::vec<itf_t>;

// Interpolates reflection infos to a single point using idw.
itf_t interpolateValues(l::deg searchRadius, ReflectionInfos::rc infos,
                        l::deg alpha, l::deg beta);

ReflectionInfos interpolate(ReflectionInfos::rc,
    l::deg alphaStep, l::deg betaStep, l::deg idwRadius,
    l::deg averagingAlphaMax, l::deg averagingRadius, real inclusionTreshold);

//------------------------------------------------------------------------------
}}
// eof
