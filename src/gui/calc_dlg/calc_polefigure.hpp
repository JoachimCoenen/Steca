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
#include "calc_dialog.hpp"

namespace gui { namespace calc_dlg {
//------------------------------------------------------------------------------

dcl_sub_(PoleFigure, Frame)
  PoleFigure(QWidget*, Hub::rc);

//protected:
//  TabGraph           *tabGraph_;
//  TabPoleFiguresSave *tabSave_;

//  void displayReflection(uint reflIndex, bool interpolated);

//  bool savePoleFigureOutput();
//  bool writePoleFigureOutputFiles(rcstr filePath, uint index);
//  void writePoleFile(rcstr filePath,  calc::ReflectionInfos, qreal_vec::rc);
//  void writeListFile(rcstr filePath,  calc::ReflectionInfos, qreal_vec::rc);
//  void writeErrorMask(rcstr filePath, calc::ReflectionInfos, qreal_vec::rc);
dcl_end

//------------------------------------------------------------------------------
}}
// eof
