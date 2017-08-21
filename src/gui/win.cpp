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
#include "../manifest.h"
#include "app.hpp"
#include "panel/inc.inc"
#include "settings.hpp"

#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStatusBar>
#include <QtMultimedia/QSound>

#include <lib/qt/inc/defs.inc>
#include <lib/qt/dlg_about.hpp>
#include <lib/qt/dlg_msg.hpp>
#include <lib/qt/wgt_inc.hpp>

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

  tb->add(new l_qt::actbtn(a.get(a.SESSION_LOAD)));
  tb->add(new l_qt::actbtn(a.get(a.SESSION_SAVE)));
  tb->addSpacing(8);

#ifndef Q_OS_OSX
  tb->add(new l_qt::actbtn(a.get(a.FULL_SCREEN)));
#endif
  tb->addSpacing(8);
  tb->add(new l_qt::actbtn(a.get(a.ABOUT)));
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

namespace  {

dcl_sub_(DlgAbout, l_qt::DlgAbout)
  using base::base;

  mth_(str,      infoText, ());
  mth_(QLayout*, extra,    ());

  mutable l_qt::chk *chkAbout, *chkUpdate;

  void accept();
  void mouseDoubleClickEvent(QMouseEvent*);
dcl_end

str DlgAbout::infoText() const {
  str const arch =
#if defined(__x86_64__) || defined(_WIN64)
  " (64b)";
#else
  "";
#endif

  return  CAT(
    "<h4>", APPLICATION_NAME, " ver. ", APPLICATION_VERSION, arch, "</h4>",
    "<p>StressTextureCalculator</p>",
    "<p>Copyright: Forschungszentrum Jülich GmbH 2016-2017</p>",
    "<p><a href='", STECA2_PAGES_URL, "'>", STECA2_PAGES_URL, "</a></p>");
  }
}

QLayout* DlgAbout::extra() const {
  auto vb = new l_qt::vbox;
  auto& hb = vb->hb();

  hb.addWidget(new l_qt::lbl("at startup: "));
  hb.addWidget((chkAbout  = new l_qt::chk("&show this dialog")));
  hb.addWidget((chkUpdate = new l_qt::chk("&check for update")));
  hb.addStretch();

  using S = Settings;
  S s(S::GROUP_CONFIG);

  chkAbout->check(s.readBool(S::STARTUP_ABOUT, true));
  chkUpdate->check(s.readBool(S::STARTUP_UPDATE, true));

  //// TODO put back
  // auto& gr = vb->gr();
  ////  g->addWidget(label("default det. distance"), 0, 0, Qt::AlignRight);
  //  g->addWidget((detDistance_ = spinDoubleCell(gui_cfg::em4_2, typ::Geometry::MIN_DETECTOR_DISTANCE)), 0, 1);
  ////  g->addWidget(label("default pixel size"), 1, 0, Qt::AlignRight);
  //  g->addWidget((detPixelSize_ = spinDoubleCell(gui_cfg::em4_2, typ::Geometry::MIN_DETECTOR_PIXEL_SIZE)), 1, 1);
  //  g->addColumnStretch();

  //  detPixelSize_->setDecimals(3);
  //  detDistance_->setValue(s.readReal(config_key::DET_DISTANCE, typ::Geometry::DEF_DETECTOR_DISTANCE));
  //  detPixelSize_->setValue(s.readReal(config_key::DET_PIX_SIZE, typ::Geometry::DEF_DETECTOR_PIXEL_SIZE));

  //// TODO put back
  //  detDistance_->setVisible(false);
  //  detPixelSize_->setVisible(false);

  return vb;
}

void DlgAbout::accept() {
  using S = Settings;
  S s(S::GROUP_CONFIG);

  s.saveBool(S::STARTUP_ABOUT,  chkAbout->isChecked());
  s.saveBool(S::STARTUP_UPDATE, chkUpdate->isChecked());

//TODO  s.saveReal(config_key::DET_DISTANCE, detDistance_->value());
//  s.saveReal(config_key::DET_PIX_SIZE, detPixelSize_->value());

  base::accept();
}

void DlgAbout::mouseDoubleClickEvent(QMouseEvent *) {
  QSound::play(":/HAL/good_evening");
}

void Win::about() {
  DlgAbout(this).exec();
}

void Win::onFirstShow() {
  checkActions();

  using S = Settings;
  S s(S::GROUP_CONFIG);

  auto ver = APPLICATION_VERSION;
  if (ver != s.readStr(Settings::CURRENT_VERSION)) {
    // we have a newer (or older) version
    s.saveStr(S::CURRENT_VERSION, ver);
    s.saveBool(S::STARTUP_UPDATE, true);
    s.saveBool(S::STARTUP_ABOUT,  true);
  }

  if (s.readBool(S::STARTUP_UPDATE, true))
    checkUpdate(false);
  if (s.readBool(S::STARTUP_ABOUT, true))
    about();

#ifdef DEVELOPMENT
  hub.addFiles({
    "/home/jan/C/+dev/fz/data/SS15/m280580.dat"
  });
//  hub.addFile(l_io::path("/home/jan/C/+dev/fz/data/SS15/m280581.dat"));
#endif
}

bool Win::onClose() {
  return true; // TODO dlgYes("Quit?");
}

void Win::checkUpdate(bool completeReport) const {
  App::NoWarnings __;

  QNetworkRequest req;

  QString const ver = APPLICATION_VERSION;

  auto qry = ver + "\t| " + QSysInfo::prettyProductName();
  req.setUrl(QUrl(QString(STECA2_VERSION_URL) + "?" + qry));
  auto reply = netMan.get(req);

  connect(reply, &QNetworkReply::finished, [this, completeReport, reply]() {
    reply->deleteLater();
    if (QNetworkReply::NoError == reply->error()) {
      auto lastVer = reply->readAll().trimmed().toStdString();

      auto ver  = APPLICATION_VERSION;
      auto name = APPLICATION_NAME;

      if (ver != lastVer)
        l_qt::dlgInfo(mutp(this),
          CAT("<p>The latest released ", name, " version is ", lastVer,
              ". You have version ", ver, ".</p>",
              "<p><a href='", STECA2_DOWNLOAD_URL, "'>Get new ", name, "</a></p>"));
      else if (completeReport)
        l_qt::dlgInfo(mutp(this),
          CAT("<p>You have the latest released ", name, " version (", ver, ").</p>"));
    }
  });
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
