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

#include "polefigure.hpp"
#include <lib/dev/inc/defs.inc>
#include <lib/dev/io/log.hpp>
#include <algorithm>

namespace core { namespace calc {
//------------------------------------------------------------------------------

itf_t::itf_t()
: itf_t(inten_t(), tth_t(), fwhm_t()) {}

itf_t::itf_t(inten_t inten_, tth_t tth_, fwhm_t fwhm_)
: inten(inten_), tth(tth_), fwhm(fwhm_) {}

itf_t::ref itf_t::operator=(rc that) {
  mut(inten) = that.inten;
  mut(tth)   = that.tth;
  mut(fwhm)  = that.fwhm;
  RTHIS
}

itf_t::ref itf_t::operator+=(rc that) {
  mut(inten) += that.inten;
  mut(tth)    = tth + that.tth;
  mut(fwhm)  += that.fwhm;
  RTHIS
}


//using namespace typ;

using info_vec = l::vec<ReflectionInfo const*>;

// Calculates the difference of two angles. Parameters should be in [0, 360].
static l::deg calculateDeltaBeta(l::deg beta1, l::deg beta2) {
  // Due to cyclicity of angles (360 is equivalent to 0), some magic is needed.
  real deltaBeta = beta1 - beta2;
  real tempDelta = deltaBeta - 360;

  if (l::abs(tempDelta) < l::abs(deltaBeta))
    deltaBeta = tempDelta;

  tempDelta = deltaBeta + 360;

  if (l::abs(tempDelta) < l::abs(deltaBeta))
    deltaBeta = tempDelta;

  ENSURE_(-180 <= deltaBeta && deltaBeta <= 180)
  return l::deg(deltaBeta);
}

// Calculates the angle between two points on a unit sphere.
static l::deg angle(l::deg alpha1, l::deg alpha2, l::deg deltaBeta) {
  // Absolute value of deltaBeta is not needed because cos is an even function.
  auto a = l::rad(acos(cos(alpha1.toRad()) * cos(alpha2.toRad()) +
                       sin(alpha1.toRad()) * sin(alpha2.toRad()) * cos(deltaBeta.toRad()))).toDeg();
  ENSURE_(0 <= a && a <= 180)
  return a;
}

enum class eQuadrant {
  NORTHEAST,
  SOUTHEAST,
  SOUTHWEST,
  NORTHWEST,
};

static uint NUM_QUADRANTS = 4;

using Quadrants = l::vec<eQuadrant>;

static Quadrants allQuadrants() {
  return {eQuadrant::NORTHEAST, eQuadrant::SOUTHEAST, eQuadrant::SOUTHWEST,
          eQuadrant::NORTHWEST};
}

static bool inQuadrant(eQuadrant quadrant, l::deg deltaAlpha, l::deg deltaBeta) {
  switch (quadrant) {
  case eQuadrant::NORTHEAST:
    return deltaAlpha >= 0 && deltaBeta >= 0;
  case eQuadrant::SOUTHEAST:
    return deltaAlpha >= 0 && deltaBeta < 0;
  case eQuadrant::SOUTHWEST:
    return deltaAlpha < 0 && deltaBeta < 0;
  case eQuadrant::NORTHWEST:
    return deltaAlpha < 0 && deltaBeta >= 0;
  }

  NEVER return false;
}

// Search quadrant remapping in case no point was found.
eQuadrant remapQuadrant(eQuadrant q) {
  switch (q) {
  case eQuadrant::NORTHEAST:
    return eQuadrant::NORTHWEST;
  case eQuadrant::SOUTHEAST:
    return eQuadrant::SOUTHWEST;
  case eQuadrant::SOUTHWEST:
    return eQuadrant::NORTHEAST;
  case eQuadrant::NORTHWEST:
    return eQuadrant::SOUTHEAST;
  }

  NEVER return eQuadrant::NORTHEAST;
}

// Checks if (alpha,beta) is inside radius from (centerAlpha,centerBeta).
static bool inRadius(l::deg alpha, l::deg beta,
                     l::deg centerAlpha, l::deg centerBeta, l::deg radius) {
  real a = angle(alpha, centerAlpha, calculateDeltaBeta(beta, centerBeta));
  return l::abs(a) < radius;
}


// Adds data from reflection infos within radius from alpha and beta
// to the peak parameter lists.
static void searchPoints(l::deg alpha, l::deg beta, l::deg radius,
                         ReflectionInfos::rc infos, itfs_t& itfs) {
  // REVIEW Use value trees to improve performance.
  for (auto& info : infos) {
    if (inRadius(info.alpha, info.beta, alpha, beta, radius))
      itfs.add(itf_t(info.inten, info.tth, info.fwhm));
  }
}

// Searches closest ReflectionInfos to given alpha and beta in quadrants.
static void searchInQuadrants(Quadrants::rc quadrants,
    l::deg alpha, l::deg beta, l::deg searchRadius,
    ReflectionInfos::rc infos, info_vec& foundInfos, real_vec& distances) {
  ENSURE_(quadrants.size() <= NUM_QUADRANTS);
  // Take only reflection infos with beta within +/- BETA_LIMIT degrees into
  // account. Original STeCa used something like +/- 1.5*36 degrees.
  real const BETA_LIMIT = 30;
  distances  = real_vec(quadrants.size(), l::flt_inf);
  foundInfos = info_vec(quadrants.size(), nullptr);
  // Find infos closest to given alpha and beta in each quadrant.
  for (auto& info : infos) {
    // REVIEW We could do better with value trees than looping over all infos.
    auto deltaBeta = calculateDeltaBeta(info.beta, beta);
    if (l::abs(deltaBeta) > BETA_LIMIT) continue;
    auto deltaAlpha = info.alpha - alpha;
    // "Distance" between grid point and current info.
    auto d = angle(alpha, info.alpha, deltaBeta);
    for_i_(quadrants.size()) {
      if (inQuadrant(quadrants.at(i), deltaAlpha, deltaBeta)) {
        if (d >= distances.at(i)) continue;
        distances.setAt(i, d);
        if (l::isnan(searchRadius) || d < searchRadius) {
          foundInfos.setAt(i, &info);
        }
      }
    }
  }
}

itf_t inverseDistanceWeighing(real_vec::rc distances, info_vec::rc infos) {
  itf_t itf;
  uint N = NUM_QUADRANTS;
  // Generally, only distances.count() == values.count() > 0 is needed for this
  // algorithm. However, in this context we expect exactly the following:
  check_or_err_(distances.size() == N, "distances size should be 4");
  check_or_err_(infos.size() == N, "infos size should be 4");
  real_vec inverseDistances(N);
  real     inverseDistanceSum = 0;
  for_i_(NUM_QUADRANTS) {
    if (distances.at(i) == .0) {
      // Points coincide; no need to interpolate.
      auto& in  = infos.at(i);
      mut(itf.inten) = inten_t(in->inten);
      mut(itf.tth)   = in->tth;
      mut(itf.fwhm)  = in->fwhm;
      return itf;
    }
    inverseDistances.setAt(i, 1 / distances.at(i));
    inverseDistanceSum += inverseDistances.at(i);
  }
  // REVIEW The RAW peak may need different handling.
  real offset = 0;
  real height = 0;
  real fwhm   = 0;
  for_i_(N) {
    auto& in = infos.at(i);
    auto& d  = inverseDistances.at(i);
    offset += in->tth   * d;
    height += in->inten * d;
    fwhm   += in->fwhm  * d;
  }

  mut(itf.inten) = inten_t(height / inverseDistanceSum);
  mut(itf.tth)   = tth_t(offset / inverseDistanceSum);
  mut(itf.fwhm)  = fwhm_t(fwhm / inverseDistanceSum);
  return itf;
}

itf_t interpolateValues(l::deg searchRadius, ReflectionInfos::rc infos,
                        l::deg alpha, l::deg beta) {
  info_vec interpolationInfos;
  real_vec distances;
  searchInQuadrants(allQuadrants(), alpha, beta, searchRadius,
                    infos, interpolationInfos, distances);
  // Check that infos were found in all quadrants.
  uint numQuadrantsOk = 0;
  for_i_(NUM_QUADRANTS) {
    if (interpolationInfos.at(i)) {
      ++numQuadrantsOk;
      continue;
    }
    // No info found in quadrant? Try another quadrant. See
    // [J.Appl.Cryst.(2011),44,641] for the angle mapping.
    eQuadrant newQ = remapQuadrant(eQuadrant(i));
    auto newAlpha =
        l::deg(i == uint(eQuadrant::NORTHEAST) || i == uint(eQuadrant::SOUTHEAST)
            ? 180 - alpha
            : -alpha);
    auto newBeta = l::deg(beta < 180 ? beta + 180 : beta - 180);
    info_vec renewedSearch;
    real_vec newDistance;
    searchInQuadrants({newQ}, newAlpha, newBeta, searchRadius, infos,
                      renewedSearch, newDistance);
    ENSURE_(renewedSearch.size() == 1);
    ENSURE_(newDistance.size() == 1);
    if (renewedSearch.first()) {
      interpolationInfos.setAt(i, renewedSearch.first());
      distances.setAt(i, newDistance.first());
      ++numQuadrantsOk;
    }
  }
  // Use inverse distance weighing if everything is alright.
  if (numQuadrantsOk == NUM_QUADRANTS)
    return inverseDistanceWeighing(distances, interpolationInfos);
  else
    return itf_t();
}

// Interpolates infos to equidistant grid in alpha and beta.
ReflectionInfos interpolate(ReflectionInfos::rc infos,
    l::deg alphaStep, l::deg betaStep, l::deg idwRadius,
    l::deg averagingAlphaMax, l::deg averagingRadius, real inclusionTreshold) {
  // Two interpolation methods are used here:
  // If grid point alpha <= averagingAlphaMax, points within averagingRadius
  // will be averaged.
  // If averaging fails, or alpha > averagingAlphaMax, inverse distance weighing
  // will be used.

  EXPECT_(0 <  alphaStep && alphaStep <= 90);
  EXPECT_(0 <  betaStep  && betaStep  <= 360);
  EXPECT_(0 <= averagingAlphaMax && averagingAlphaMax <= 90);
  EXPECT_(0 <= averagingRadius);
  // Setting idwRadius = NaN disables idw radius checks and falling back to
  // idw when averaging fails.
  EXPECT_(l::isnan(idwRadius) || 0 <= idwRadius);
  EXPECT_(0 <= inclusionTreshold && inclusionTreshold <= 1);

  // NOTE We expect all infos to have the same gamma range.

  // REVIEW qRound oder qCeil?
  auto numAlphas = l::to_uint(l::round(90.  / alphaStep)),
       numBetas  = l::to_uint(l::round(360. / betaStep));

  ReflectionInfos interpolatedInfos;  // Output data.
  interpolatedInfos.reserve(numAlphas * numBetas);

  for_var_(i, numAlphas + 1) {  // REVIEW why + 1
    auto const alpha = l::deg(i * alphaStep);
    for_var_(j, numBetas) {
      auto const beta = l::deg(j * betaStep);
      l_io::progress::step(i*numBetas + j, numAlphas * numBetas);

      if (infos.isEmpty()) {
        interpolatedInfos.add(ReflectionInfo(alpha, beta));
        continue;
      }

      if (alpha <= averagingAlphaMax) {
        // Use averaging.

        itfs_t itfs;
        searchPoints(alpha, beta, averagingRadius, infos, itfs);

        if (!itfs.isEmpty()) {
          // If inclusionTreshold < 1, we'll only use a fraction of largest
          // reflection parameter values.
          std::sort(itfs.begin(), itfs.end(), [](itf_t::rc i1, itf_t::rc i2) {
            return i1.inten < i2.inten;
          });

          itf_t avg(inten_t(0), tth_t(0), fwhm_t(0));

          uint iEnd   = itfs.size();
          uint iBegin = l::min(l::to_uint(l::round(itfs.size() * (1. - inclusionTreshold))), iEnd-1);
          EXPECT_(iBegin < iEnd)
          uint n = iEnd - iBegin;

          for (uint i = iBegin; i < iEnd; ++i)
            avg += itfs.at(i);

          interpolatedInfos.add(ReflectionInfo(alpha, beta, infos.first().rgeGma,
              inten_t(avg.inten / real(n)), inten_t(),
              avg.tth   / real(n), tth_t(l::flt_nan),
              fwhm_t(avg.fwhm  / real(n)), fwhm_t()));
          continue;
        }

        if (!l::isnan(idwRadius)) {
          // Don't fall back to idw, just add an unmeasured info.
          interpolatedInfos.add(ReflectionInfo(alpha, beta));
          continue;
        }
      }

      // Use idw, if alpha > averagingAlphaMax OR averaging failed (too small
      // averagingRadius?).
      itf_t itf = interpolateValues(idwRadius, infos, alpha, beta);
      interpolatedInfos.add(
        ReflectionInfo(alpha, beta, infos.first().rgeGma,
                       itf.inten, inten_t(l::flt_nan),
                       itf.tth,   tth_t(l::flt_nan),
                       itf.fwhm,  fwhm_t(l::flt_nan)));
    }
  }

  return interpolatedInfos;
}

//------------------------------------------------------------------------------
}}
// eof
