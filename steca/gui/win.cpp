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

#include "win.hpp"
#include "panel/inc.inc"
#include <dev_lib/defs.inc>
#include <qt_lib/wgt_inc.hpp>
#include <qt_lib/dlg_about.hpp>
#include <QApplication>
#include <QStatusBar>
#include <manifest>

namespace gui {
//------------------------------------------------------------------------------

Win::Win() : hub(*this) {
  panelFiles         = new PanelFiles(hub);
  panelDatasets      = new PanelDatasets(hub);
  panelSetup         = new PanelSetup(hub);
  panelImage         = new PanelImage(hub);
  panelDiffractogram = new PanelDiffractogram(hub);
  panelMetadata      = new PanelMetadata(hub);

  auto &hb = makePanel().hb(); // main: horizontal layout

  auto &sp = hb.hs();

  auto &ls  = sp.vs();  // left
  ls.add(panelFiles);
  ls.add(panelDatasets);

  auto &cs  = sp.vs();  // center
  auto &ts  = cs.hs();

  ts.add(panelSetup);
  ts.add(panelImage);
  cs.add(panelDiffractogram);

  auto &rs  = sp.vs();  // right
  rs.add(panelMetadata);

  auto &a = hub.acts;

  // status & tool bars
  auto bar = statusBar(); // make one
  bar->setSizeGripEnabled(false);

  auto tw = new QWidget;  // toolbar widget
  bar->addPermanentWidget(tw);

  auto tb = new l_qt::hbox; // toolbar
  tw->setLayout(tb);

  tb->add(new l_qt::actbtn(a.get(a.SHOW_FILES)));
  tb->add(new l_qt::actbtn(a.get(a.SHOW_DATASETS)));
  tb->add(new l_qt::actbtn(a.get(a.SHOW_METADATA)));
  tb->addSpacing(8);

#ifndef Q_OS_OSX
  tb->add(new l_qt::actbtn(a.get(a.FULL_SCREEN)));
#endif
  tb->add(new l_qt::actbtn(a.get(a.ABOUT)));
  tb->addSpacing(8);
  tb->add(new l_qt::actbtn(a.get(a.QUIT)));

  a.get(a.SHOW_FILES).onToggle([this](bool on) {
    panelFiles->setVisible(on);
  });

  a.get(a.SHOW_DATASETS).onToggle([this](bool on) {
    panelDatasets->setVisible(on);
  });

  a.get(a.SHOW_METADATA).onToggle([this](bool on) {
    panelMetadata->setVisible(on);
  });
}

l_qt::acts const& Win::getActs() const {
  return hub.acts;
}

void Win::about() {
  QString const arch =
#if defined(__x86_64__) || defined(_WIN64)
  "(64b)";
#else
  QString::null;
#endif

  auto infoText = QString(
    "<h4>%1 ver. %2 %4</h4>"
    "<p>StressTextureCalculator</p>"
    "<p>Copyright: Forschungszentrum Jülich GmbH 2016-2017</p>"
    "<p><a href='%3'>%3</a></p>")
    .arg(qApp->applicationName()).arg(qApp->applicationVersion())
    .arg(STECA2_PAGES_URL)
    .arg(arch);

  l_qt::dlgAbout(this, l_qt::fromQt(infoText));
}

void Win::onFirstShow() {
  checkActions();
}

bool Win::onClose() {
  return true; // TODO dlgYes("Quit?");
}

void Win::checkActions() {
  auto &a = hub.acts;
#ifndef Q_OS_OSX
  a.get(a.FULL_SCREEN).check(isFullScreen());
#endif
  a.get(a.SHOW_FILES).check(panelFiles->isVisible());
  a.get(a.SHOW_DATASETS).check(panelDatasets->isVisible());
  a.get(a.SHOW_METADATA).check(panelMetadata->isVisible());
}

//------------------------------------------------------------------------------
}
// eof
