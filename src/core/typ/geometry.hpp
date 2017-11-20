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
 * Geometry: beam - sample - detector
 */
#pragma once

#include <lib/dev/inc/ptr.hpp>
#include <lib/dev/typ/ij.hpp>
#include <lib/dev/typ/sz2.hpp>

namespace core {
//------------------------------------------------------------------------------

/// Image rotation and mirroring
dcl_(ImageTransform)
  enum eTransform {
    ROTATE_0        = 0,  // no transform
    ROTATE_1        = 1,  // one quarter
    ROTATE_2        = 2,  // two quarters
    ROTATE_3        = 3,  // three quarters
    MIRROR          = 4,
    MIRROR_ROTATE_0 = MIRROR | ROTATE_0,
    MIRROR_ROTATE_1 = MIRROR | ROTATE_1,
    MIRROR_ROTATE_2 = MIRROR | ROTATE_2,
    MIRROR_ROTATE_3 = MIRROR | ROTATE_3,
  };

  atr_(eTransform, val);

  // clamps val appropriately
  ImageTransform(uint val = ROTATE_0);

  // adds/removes the mirror flag
  ImageTransform mirror(bool on) const;

  // rotates only; keeps the mirror flag
  ImageTransform rotateTo(rc) const;

  // rotates by one quarter-turn
  ImageTransform nextRotate() const;

  bol_(isTransposed, ()) RET_(0 != (val & 1))

  bol_(operator==, (rc that)) RET_(val == that.val)
dcl_end

/// Image cut
dcl_(ImageCut) COMPARABLE EQNE
  atr_(uint, left);
  atr_(uint, top);
  atr_(uint, right);
  atr_(uint, bottom);

  ImageCut();
  ImageCut(uint left, uint top, uint right, uint bottom);

  l::sz2 marginSize() const;
};

/// Detector  geometry
dcl_(Geometry) COMPARABLE EQNE SHARED
  cst_(real, MIN_DETECTOR_DISTANCE);
  cst_(real, MIN_DETECTOR_PIXEL_SIZE);
  cst_(real, DEF_DETECTOR_DISTANCE);
  cst_(real, DEF_DETECTOR_PIXEL_SIZE);

  Geometry();

  atr_(real,  detectorDistance);  ///< the distance from the sample (millimeters)
  atr_(real,  pixSize);           ///< detecor pixel size (mm)
  atr_(l::ij, midPixOffset);      ///< the offset of the beam centre (pixels)

  atr_(ImageTransform,  imageTransform);
  atr_(ImageCut,        imageCut);
  atr_(l::sz2,          imageSize);
dcl_end

//------------------------------------------------------------------------------
}
// eof
