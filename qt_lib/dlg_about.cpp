// (qt_lib)

#include "dlg_about.hpp"
#include <dev_lib/defs.inc>
#include <qt_lib/wgt_inc.hpp>

#include "str_inc.hpp"

#include <QApplication>
#include <QDialog>

//------------------------------------------------------------------------------

namespace {

dcl_sub_(AboutDialog, QDialog)
  AboutDialog(QWidget*);

protected:
  void accept();
  void mouseDoubleClickEvent(QMouseEvent*);

// TODO  QCheckBox      *cbShowAtStartup_, *cbCheckUpdatesAtStartup_;
//  QDoubleSpinBox *detDistance_, *detPixelSize_;
dcl_end

AboutDialog::AboutDialog(QWidget *parent) : base(parent, Qt::Dialog) {
//  Settings s(config_key::GROUP_CONFIG);

  int PAD = 12;

  setWindowTitle(QString("About %1").arg(qApp->applicationName()));

  // layout
  auto vb = new l_qt::vbox;
  setLayout(vb);

  vb->setSpacing(PAD);
  vb->setSizeConstraint(QLayout::SetFixedSize);

  // logo and info
  auto hb = new l_qt::hbox;;
  vb->addLayout(hb);

  hb->setSpacing(PAD);

//  auto logo = l_qt::lbl(str::null);
//  logo->setPixmap(QPixmap(":/icon/retroStier")
//                  .scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//  hb->addWidget(logo);

//#ifdef __x86_64__
//  str arch = "(64b)";
//#else
//  str arch = EMPTY_STR;
//#endif

//  auto info = label(
//      str("<h4>%1 ver. %2 %5</h4>"
//          "<p>StressTextureCalculator</p>"
//          "<p>Copyright: Forschungszentrum Jülich GmbH %3</p>"
//          "<p><a href='%4'>%4</a></p>")
//          .arg(qApp->applicationName()).arg(qApp->applicationVersion())
//          .arg(QDate::currentDate().toString("yyyy"))
//          .arg(STECA2_PAGES_URL)
//          .arg(arch));

//  info->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//  info->setOpenExternalLinks(true);
//#ifdef Q_OS_MAC
//  // a smaller font (a hint found in Qt source code)
//  info->setFont(QToolTip::font());
//#endif

//  hb->addWidget(info);

//  auto hline = []() {
//    auto frame = new QFrame;
//    frame->setFrameShape(QFrame::HLine);
//    frame->setFrameShadow(QFrame::Sunken);
//    return frame;
//  };

//  // configuration

//  vb->addWidget(hline());

//  hb = hbox();
//  vb->addLayout(hb);

//  hb->addWidget(label("at startup:"));
//  hb->addWidget((cbShowAtStartup_ = check("&show this dialog")));
//  hb->addWidget((cbCheckUpdatesAtStartup_ = check("&check for update")));
//  hb->addStretch();

//  cbShowAtStartup_->setChecked(s.readBool(config_key::STARTUP_ABOUT, true));
//  cbCheckUpdatesAtStartup_->setChecked(s.readBool(config_key::STARTUP_CHECK_UPDATE, true));

//  vb->addWidget(hline());

//  auto g = gridLayout();
//  vb->addLayout(g);

//// TODO put back
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

//  // buttons

//  vb->addWidget(hline());

//  auto bb = new QDialogButtonBox(QDialogButtonBox::Ok);
//  vb->addWidget(bb);

//  connect(bb, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

void AboutDialog::accept() {
//  Settings s(config_key::GROUP_CONFIG);

//  s.saveBool(config_key::STARTUP_ABOUT, cbShowAtStartup_->isChecked());
//  s.saveBool(config_key::STARTUP_CHECK_UPDATE, cbCheckUpdatesAtStartup_->isChecked());

//  s.saveReal(config_key::DET_DISTANCE, detDistance_->value());
//  s.saveReal(config_key::DET_PIX_SIZE, detPixelSize_->value());

  base::accept();
}

void AboutDialog::mouseDoubleClickEvent(QMouseEvent *) {
//  QSound::play(":/HAL/good_evening");
}

}

namespace l_qt {
//------------------------------------------------------------------------------

void dlgAbout(QWidget* parent) {
  AboutDialog(parent).exec();
}

//------------------------------------------------------------------------------
}
// eof