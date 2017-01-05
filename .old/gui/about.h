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

#ifndef ABOUT_H
#define ABOUT_H

#include "thehub.h"
#include "def/defs.h"

#include <QDialog>

namespace gui {
//------------------------------------------------------------------------------

class AboutBox : public QDialog {
  CLASS(AboutBox) SUPER(QDialog)
public:
  AboutBox(QWidget*);

protected:
  void accept();
  void mouseDoubleClickEvent(QMouseEvent*);

  QCheckBox      *cbShowAtStartup_, *cbCheckUpdatesAtStartup_;
  QDoubleSpinBox *detDistance_, *detPixelSize_;
};

//------------------------------------------------------------------------------
}
#endif // MAINWIN_H
