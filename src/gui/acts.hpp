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
 * GUI Actions.
 */

#pragma once

#include <lib/qt/app/act.hpp>

namespace gui {
//------------------------------------------------------------------------------

struct Hub;
struct Win;

dcl_sub_(Acts, l_qt::acts)
  Acts(Hub&, Win&);

  cst_(str, SHOW_FILES);        ///< show/hide the files panel
  cst_(str, SHOW_DATASETS);     ///< show/hide the datasets panel
  cst_(str, SHOW_METADATA);     ///< show/hide the metadata panel

  cst_(str, SESSION_LOAD);      ///< load a session file
  cst_(str, SESSION_SAVE);      ///< save a session file

  cst_(str, FILES_ADD);         ///< open a file-selection dialog
  cst_(str, FILES_REM);         ///< remove selected file
  cst_(str, CORR_ENABLE);       ///< enable correction (open a file selection dialog)
  cst_(str, CORR_REM);          ///< remove the correction file

  // diffractogram
  cst_(str, DIFF_ZOOM);         ///< zoom into diffractogram
  cst_(str, DIFF_ALL_DSETS);    ///< show the average of all datasets
  cst_(str, DIFF_FIX_INTEN);    ///< fix the intensity scale

  // image
  cst_(str, IMG_FIX_INTEN);     ///< fix the intensity scale (image)
  cst_(str, IMG_STEP_ZOOM);     ///< zoom in steps (whole multiplies or fractions of pixels)
  cst_(str, IMG_SHOW_OVER);     ///< show overlay
  cst_(str, IMG_SHOW_GAMMA);    ///< show gamma slices (and 2theta bins)

  cst_(str, IMG_ROTATE0);       ///< rotate image to initial position
  cst_(str, IMG_ROTATE1);       ///< rotate image by 90 degrees
  cst_(str, IMG_ROTATE2);       ///< rotate image by 180 degrees
  cst_(str, IMG_ROTATE3);       ///< rotate image by 270 degrees

  cst_(str, IMG_MIRROR_HORZ);   ///< horizontally mirror the image
  cst_(str, IMG_MIRROR_VERT);   ///< vertically mirror the image

  cst_(str, IMG_LINK_CUT);      ///< link the cut values

  // background
  cst_(str, BG_SELECT);         ///< background-selection tool
  cst_(str, BG_SHOW);           ///< show/hide the background criteria
  cst_(str, BG_CLEAR);          ///< clear the background sections

  // reflection
  cst_(str, REFL_ADD);          ///< add (select the range of) a reflection
  cst_(str, REFL_REM);          ///< remove a reflection
  cst_(str, REFL_SELECT);       ///< reflection-selection tool
  cst_(str, REFL_CLEAR);        ///< clear the reflections

  // outputs
  cst_(str, DLG_POLEFIG);       ///< open a new polefigure-calculating dialog
  cst_(str, DLG_DIAGRAM);       ///< open a new diagram-calculating dialog
  cst_(str, DLG_DIFFRGS);       ///< open a new diffractogram-output dialog
dcl_end

//------------------------------------------------------------------------------
}
// eof
