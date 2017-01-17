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

#ifndef GUI_APP_H
#define GUI_APP_H

#include <c/qt/app.hpp>

#define DATA_NS   gui
#define DATA_NAME App

_struct_sub (c_qt::app)
  App(int& argc, char* argv[]);
  int exec();
_struct_sub_end

#undef DATA_NAME
#undef DATA_NS

#endif
// eof
