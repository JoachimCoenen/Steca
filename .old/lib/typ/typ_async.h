/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
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

#ifndef TYP_ASYNC_H
#define TYP_ASYNC_H

#include "def/defs.h"

class TakesLongTime final {
public:
  TakesLongTime();
 ~TakesLongTime();

  static void (*handler)(bool);
};

class Progress final {
public:
  Progress(uint mulTotal, class QProgressBar*);
 ~Progress();

  void setTotal(uint);
  void setProgress(uint);
  void step();

private:
  uint total_, mulTotal_, i_;
  QProgressBar* bar_;
};

#endif // TYP_ASYNC_H
