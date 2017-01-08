/*******************************************************************************
 * REVIEW: STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016 Forschungszentrum Jülich GmbH 2016
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

#ifndef APP_H
#define APP_H

#include "def/defs.h"

//------------------------------------------------------------------------------

class NoWarnings {
public:
  NoWarnings();
 ~NoWarnings();
};


private:
  // Exceptions caught here; displayed in a dialog.
  bool notify(QObject*, QEvent*);
};

//------------------------------------------------------------------------------
#endif // APP_H
